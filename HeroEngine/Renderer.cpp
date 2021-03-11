#include "Renderer.h"
#include <iostream>
#include <list>

void drawTriangleFast(const IndependendFace& face, sf::RenderTarget &image) {
	sf::ConvexShape triangle = sf::ConvexShape(3);
	float posX = (float)image.getSize().x / 2.0f;
	float posY = (float)image.getSize().y / 2.0f;
	triangle.setPosition(posX, posY);
	triangle.setPoint(0, sf::Vector2f(face.A.x + posX, face.A.y + posY));
	triangle.setPoint(1, sf::Vector2f(face.B.x + posX, face.B.y + posY));
	triangle.setPoint(2, sf::Vector2f(face.C.x + posX, face.C.y + posY));
	triangle.setFillColor(sf::Color::Transparent);
	triangle.setOutlineThickness(1);
	triangle.setOutlineColor(sf::Color::White);
	image.draw(triangle);

}

void ClipAgainsPlane(const Vector3d& plane_p, Vector3d plane_n, const IndependendFace& face, std::vector<IndependendFace>& resoult) {
	
	Vector3d normalN = Vector3d::Normalize(plane_n);

	Vector3d insidePoints[3];  int ninsideP = 0;
	Vector3d outsidePoints[3];  int noutsideP = 0;

	auto dist = [&] (const Vector3d & vector) {
		return normalN.x * vector.x + normalN.y * vector.y + normalN.z * vector.z - normalN.DotProduct(plane_p);
	};

	if (dist(face.A) >= 0.0f) { insidePoints[ninsideP++] = face.A; }
	else { outsidePoints[noutsideP++] = face.A; }

	if (dist(face.B) >= 0.0f) { insidePoints[ninsideP++] = face.B; }
	else { outsidePoints[noutsideP++] = face.B; }

	if (dist(face.C) >= 0.0f) { insidePoints[ninsideP++] = face.C; }
	else { outsidePoints[noutsideP++] = face.C; }

	switch (ninsideP) {
	case 0: break;
	case 1: 
		resoult.push_back(IndependendFace(insidePoints[0],
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[0]),
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[1])));

		break;
	case 2:
		resoult.push_back(IndependendFace(insidePoints[0],
			insidePoints[1],
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[0])));

		resoult.push_back(
			IndependendFace(insidePoints[1],
				Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[0]),
				Vector3d::IntersectPlane(plane_p, normalN, insidePoints[1], outsidePoints[0])));
		break;
	case 3:
		resoult.push_back(face);
		break;
	default:break;
	}

}

void Renderer::Render(const Scene &scene, sf::RenderTarget& img, bool debug)
{

	if (debug) {
		std::cout << "=========== DEBUG ===========" << std::endl;
	}

	float scale = 0.5f;
	float width = (float)img.getSize().x * scale;
	float height = (float)img.getSize().y * scale;

	std::vector<IndependendFace> drawBuffor = std::vector<IndependendFace>();
	std::vector<IndependendFace> clipped = std::vector<IndependendFace>();

	for (Mesh mesh : scene.Objects) {

		if (debug) {
			std::cout << mesh.Position.ToString("Position") << std::endl;
			std::cout << mesh.Rotation.ToString("Rotation") << std::endl;
		}

		TransformMatrix matRotZ = TransformMatrix(), matRotX = TransformMatrix(), matRotY = TransformMatrix();
		matRotZ.MakeRotationZ(mesh.Rotation.z);
		matRotX.MakeRotationX(mesh.Rotation.x);
		matRotY.MakeRotationY(mesh.Rotation.y);

		TransformMatrix transformMatrix = TransformMatrix();
		transformMatrix.MakeTranslation(mesh.Position.x, mesh.Position.y, mesh.Position.z);

		TransformMatrix worldMatrix = TransformMatrix(matRotZ);
		worldMatrix.Multiply(matRotX);
		worldMatrix.Multiply(matRotY);
		worldMatrix.Multiply(transformMatrix);

		for (Face face : mesh.Faces) {

			Vector3d triFace[] = { *face.A,*face.B,*face.C };

			triFace[0] = worldMatrix.Multiply(triFace[0]);
			triFace[1] = worldMatrix.Multiply(triFace[1]);
			triFace[2] = worldMatrix.Multiply(triFace[2]);

			Vector3d line1 = Vector3d::Sub(triFace[1], triFace[0]);
			Vector3d line2 = Vector3d::Sub(triFace[2], triFace[0]);

			Vector3d normal = *line1.CrossProduct(line2)->Normalize();
			Vector3d cameraRay = Vector3d::Sub(triFace[1], scene.Cam.position);

			if (normal.DotProduct(cameraRay) <= 0.0f) {

				if (debug) {
					std::cout << face.A->ToString("A") << std::endl;
					std::cout << face.B->ToString("B") << std::endl;
					std::cout << face.C->ToString("C") << std::endl;
					std::cout << "worldMatrix" << std::endl;
					std::cout << triFace[0].ToString("TriFaceA") << std::endl;
					std::cout << triFace[1].ToString("TriFaceB") << std::endl;
					std::cout << triFace[2].ToString("TriFaceC") << std::endl;
				}

				triFace[0] = cameraMatrix.Multiply(triFace[0]);
				triFace[1] = cameraMatrix.Multiply(triFace[1]);
				triFace[2] = cameraMatrix.Multiply(triFace[2]);

				if (debug) {
					std::cout << "cameraMatrix" << std::endl;
					std::cout << triFace[0].ToString("TriFaceA") << std::endl;
					std::cout << triFace[1].ToString("TriFaceB") << std::endl;
					std::cout << triFace[2].ToString("TriFaceC") << std::endl;
				}

				clipped.clear();
				ClipAgainsPlane(Vector3d(0, 0, scene.Cam.Near), Vector3d(0, 0, 1), IndependendFace(triFace[0], triFace[1], triFace[2]), clipped);
				clipped.push_back(IndependendFace(triFace[0], triFace[1], triFace[2]));

				for (IndependendFace clippedFace : clipped) {
					clippedFace.A = projectionMatrix.Multiply(triFace[0]);
					clippedFace.B = projectionMatrix.Multiply(triFace[1]);
					clippedFace.C = projectionMatrix.Multiply(triFace[2]);

					clippedFace.A.Divide(clippedFace.A.w);
					clippedFace.B.Divide(clippedFace.B.w);
					clippedFace.C.Divide(clippedFace.C.w);
					
					clippedFace.A.x *= width;
					clippedFace.A.y *= height;

					clippedFace.B.x *= width;
					clippedFace.B.y *= height;

					clippedFace.C.x *= width;
					clippedFace.C.y *= height;

					if (clippedFace.AverageZ() > scene.Cam.Near) {
						drawBuffor.push_back(clippedFace);
					}
				} // Foreach clippedFace

			} // if dotproduct

		} // Foreach Face

	} // Foreach mesh

	std::sort(std::begin(drawBuffor), std::end(drawBuffor), [](IndependendFace faceA, IndependendFace faceB) {
		return faceA.AverageZ() < faceB.AverageZ();
		}
	); // Sort


	for (IndependendFace toDraw : drawBuffor) {

		//drawTriangleFast(toDraw, img); continue;

		std::vector<IndependendFace> clipByEdges = std::vector<IndependendFace>();
		std::list<IndependendFace> toClip = std::list<IndependendFace>();

		toClip.push_back(toDraw);
		int nNew = 1;

		for (int i = 0; i < 4; i++) {

			clipByEdges.clear();

			while (nNew > 0) {

				IndependendFace test = toClip.front();
				toClip.pop_front();

				nNew--;

				switch (i) {
				case 0: ClipAgainsPlane(Vector3d(0.0f, -(float)img.getSize().y - 1, 0.0f), Vector3d(0.0f, 1.0f, 0.0f), test, clipByEdges); break;
				case 1: ClipAgainsPlane(Vector3d(0.0f, 1.0f, 0.0f), Vector3d(0.0f, -1.0f, 0.0f), test, clipByEdges); break;
				case 2: ClipAgainsPlane(Vector3d(-(float)img.getSize().x - 1, 0.0f, 0.0f), Vector3d(1.0f, 0.0f, 0.0f), test, clipByEdges); break;
				case 3: ClipAgainsPlane(Vector3d(1.0f, 0.0f, 0.0f), Vector3d(-1.0f, 0.0f, 0.0f), test, clipByEdges); break;
				}

				for (IndependendFace face : clipByEdges) {
					toClip.push_back(face);
				}

			} //while nNew > 0

			nNew = toClip.size();
		} // for i < 4

		for (IndependendFace face : toClip) {
			drawTriangleFast(face, img);
		} // Foreach face

	} //Foreach toDraw
}

void Renderer::UpdateCamera(const Camera& camera)
{
	TransformMatrix cameraRotationMatrix = TransformMatrix();
	cameraRotationMatrix.MakeRotationY(camera.Yaw);
	Vector3d lookDirection = cameraRotationMatrix.Multiply(camera.lookDir);
	cameraMatrix.PointAt(camera.position, Vector3d::Add(camera.position, lookDirection), camera.upDirection);
	cameraMatrix.QuickInverse(cameraMatrix);
}

void Renderer::SetCamera(const Camera& camera, float imgY, float imgX)
{
	projectionMatrix.MakeProjection(camera.angle, (imgY / imgX), camera.Near, camera.Far);
}

Renderer::Renderer()
{
	this->frameRate = 30;
	this->showEdges = true;
	this->showFaces = true;
	this->cameraMatrix = TransformMatrix();
	this->projectionMatrix = TransformMatrix();
}

Renderer::Renderer(const Renderer& other)
{
	this->frameRate = other.frameRate;
	this->showEdges = other.showEdges;
	this->showFaces = other.showFaces;
	this->cameraMatrix = other.cameraMatrix;
	this->projectionMatrix = other.projectionMatrix;
}

Renderer::~Renderer()
{
}
