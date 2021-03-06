#include "Renderer.h"
#include <iostream>
#include <list>

float distance(Vector3d vectorN, const Vector3d& vectorF, const Vector3d& vectorP) {

	return vectorN.x * vectorP.x + vectorN.y * vectorP.y + vectorN.z * vectorP.z - vectorN.DotProduct(vectorF);
}

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

void drawFast(const IndependendFace& face, sf::RenderTarget& image) {
	sf::ConvexShape triangle = sf::ConvexShape(3);
	float posX = (float)image.getSize().x / 2.0f;
	float posY = (float)image.getSize().y / 2.0f;
	triangle.setPosition(posX, posY);
	triangle.setPoint(0, sf::Vector2f(face.A.x, face.A.y));
	triangle.setPoint(1, sf::Vector2f(face.B.x, face.B.y));
	triangle.setPoint(2, sf::Vector2f(face.C.x, face.C.y));
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

	float d0 = dist(face.A);
	float d1 = dist(face.B);
	float d2 = dist(face.C);

	if (d0 >= 0.0f) { insidePoints[ninsideP++] = face.A; }
	else { outsidePoints[noutsideP++] = face.A; }

	if (d1 >= 0.0f) { insidePoints[ninsideP++] = face.B; }
	else { outsidePoints[noutsideP++] = face.B; }

	if (d2 >= 0.0f) { insidePoints[ninsideP++] = face.C; }
	else { outsidePoints[noutsideP++] = face.C; }

	if (ninsideP < 3) {
		std::cout << "inside: "<<ninsideP << std::endl;
	}
	else {
		std::cout << "{ " << d0 << " , " << d1 << " , " << d2 << " }" << std::endl;
	}

	if (ninsideP == 0) {
		return;
	}
	
	if (ninsideP == 3) {
		resoult.push_back(face);
		return;
	}
	
	if (ninsideP == 1 && noutsideP == 2) {

		resoult.push_back(IndependendFace(insidePoints[0],
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[0]),
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[1])));

		return;
	}

	if (ninsideP == 2 && noutsideP == 1) {

		IndependendFace tri1 = IndependendFace(insidePoints[0],
			insidePoints[1],
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[0], outsidePoints[0]));

		resoult.push_back(tri1);

		resoult.push_back(
			IndependendFace(insidePoints[1],
			tri1.C,
			Vector3d::IntersectPlane(plane_p, normalN, insidePoints[1], outsidePoints[0])));

		return;
	}
}

void EasyRender(const Scene& scene, sf::RenderTarget& img) {

	img.clear(sf::Color::Black);
	
	TransformMatrix projectionMatrix = TransformMatrix();
	projectionMatrix.MakeProjection(scene.Cam.angle, ((float)img.getSize().y / (float)img.getSize().x), scene.Cam.Near, scene.Cam.Far);
	float scale = 0.5f;
	float width = (float)img.getSize().x * scale;
	float height = (float)img.getSize().y * scale;

	std::vector<IndependendFace> drawBuffor = std::vector<IndependendFace>();
	std::vector<IndependendFace> clipped = std::vector<IndependendFace>();
	
	for (Mesh mesh : scene.Objects) {
		
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

		TransformMatrix cameraRotationMatrix = TransformMatrix();
		cameraRotationMatrix.MakeRotationY(scene.Cam.Yaw);
		Vector3d lookDirection = cameraRotationMatrix.Multiply(scene.Cam.lookDir);
		TransformMatrix cameraMatrix = TransformMatrix();

		cameraMatrix.PointAt(scene.Cam.position, Vector3d::Add(scene.Cam.position, lookDirection), scene.Cam.upDirection);

		cameraMatrix.QuickInverse(cameraMatrix);
		
		for (Face face : mesh.Faces) {

			Vector3d triFace[] = { *face.A,*face.B,*face.C };

			triFace[0] = worldMatrix.Multiply(triFace[0]);
			triFace[1] = worldMatrix.Multiply(triFace[1]);
			triFace[2] = worldMatrix.Multiply(triFace[2]);

			Vector3d line1 = Vector3d::Sub(triFace[1], triFace[0]);
			Vector3d line2 = Vector3d::Sub(triFace[2], triFace[0]);

			Vector3d normal = *line1.CrossProduct(line2)->Normalize();
			Vector3d cameraRay = Vector3d::Sub(triFace[1], scene.Cam.position);

			if (normal.DotProduct(cameraRay) <= 0.0f ) {

				triFace[0] = cameraMatrix.Multiply(triFace[0]);
				triFace[1] = cameraMatrix.Multiply(triFace[1]);
				triFace[2] = cameraMatrix.Multiply(triFace[2]);

				clipped.clear();
				//ClipAgainsPlane(Vector3d(0, 0, scene.Cam.Near), Vector3d(0, 0, 1), IndependendFace(triFace[0], triFace[1], triFace[2]), clipped);
				clipped.push_back(IndependendFace(triFace[0], triFace[1], triFace[2]));
				for (IndependendFace clippedFace : clipped) {
					clippedFace.A = projectionMatrix.Multiply(triFace[0]);
					clippedFace.B = projectionMatrix.Multiply(triFace[1]);
					clippedFace.C = projectionMatrix.Multiply(triFace[2]);

					/*
					clippedFace.A.Divide(clippedFace.A.w);
					clippedFace.B.Divide(clippedFace.B.w);
					clippedFace.C.Divide(clippedFace.C.w);
					

					clippedFace.A.Transform(1, 1, 0);
					clippedFace.B.Transform(1, 1, 0);
					clippedFace.C.Transform(1, 1, 0);
					*/

					clippedFace.A.x *= width;
					clippedFace.A.y *= height;

					clippedFace.B.x *= width;
					clippedFace.B.y *= height;

					clippedFace.C.x *= width;
					clippedFace.C.y *= height;

					if ( clippedFace.AverageZ() > scene.Cam.Near) {
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
		int nNew =  1;
		
		for (int i = 0; i < 4; i++) {
			
			clipByEdges.clear();

			while (nNew > 0) {
				
				IndependendFace test = toClip.front();
				toClip.pop_front();
				
				nNew--;
				
				switch (i) {
				case 0: clipByEdges.push_back(test); break;//ClipAgainsPlane(Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f), test, clipByEdges); break;
				case 1: clipByEdges.push_back(test); break;//ClipAgainsPlane(Vector3d(0.0f, (float)img.getSize().y*0.5f -1, 0.0f), Vector3d(0.0f, -1.0f, 0.0f), test, clipByEdges); break;
				case 2: clipByEdges.push_back(test); break;//ClipAgainsPlane(Vector3d(0.0f, 0.0f, 0.0f), Vector3d(1.0f, 0.0f, 0.0f), test, clipByEdges); break;
				case 3: clipByEdges.push_back(test); break;//ClipAgainsPlane(Vector3d((float)img.getSize().x*0.25f - 1.0f, 0.0f, 0.0f), Vector3d(-1.0f, 0.0f, 0.0f), test, clipByEdges); break;
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

void Renderer::Render(const Scene &scene, sf::RenderTarget& img)
{
	EasyRender(scene, img); 
	return;
	
	img.clear(sf::Color::Black);
	TransformMatrix projectionMatrix = TransformMatrix();
	projectionMatrix.MakeProjection(scene.Cam.angle / 2.0f, ((float)img.getSize().y / (float)img.getSize().x), scene.Cam.Near, scene.Cam.Far);
	float scale = 0.5f;
	float width = img.getSize().x * scale;
	float height = img.getSize().y * scale;
	std::vector<IndependendFace> drawBuffor = std::vector<IndependendFace>();
	std::vector<IndependendFace> clipped = std::vector<IndependendFace>();

	for (Mesh mesh : scene.Objects) {

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

		TransformMatrix cameraRotationMatrix = TransformMatrix();
		cameraRotationMatrix.MakeRotationY(scene.Cam.Yaw);
		Vector3d lookDirection = cameraRotationMatrix.Multiply(scene.Cam.lookDir);
		TransformMatrix cameraMatrix = TransformMatrix();

		cameraMatrix.PointAt(scene.Cam.position, Vector3d::Add(scene.Cam.position,scene.Cam.lookDir), scene.Cam.upDirection);
		
		cameraMatrix.QuickInverse(cameraMatrix);

		for (Face face : mesh.Faces) {
			

			Vector3d triProjected[] = { Vector3d(),Vector3d(),Vector3d() };
			Vector3d triTransformed[] = { Vector3d(),Vector3d(),Vector3d() };
			Vector3d triViewed[] = { Vector3d(),Vector3d(),Vector3d() };

			triTransformed[0] = worldMatrix.Multiply(*face.A);
			triTransformed[1] = worldMatrix.Multiply(*face.B);
			triTransformed[2] = worldMatrix.Multiply(*face.C);

			Vector3d normal = *Vector3d::Sub(triTransformed[1],triTransformed[0]).CrossProduct(Vector3d::Sub(triTransformed[2], triTransformed[0]))->Normalize();
			Vector3d cameraRay = Vector3d::Sub(triTransformed[0], scene.Cam.position);
			
			
			if (normal.DotProduct(cameraRay) < 0.0 || true) {
				// TO DO: Shading here

				triViewed[0] = cameraMatrix.Multiply(triTransformed[0]);
				triViewed[1] = cameraMatrix.Multiply(triTransformed[1]);
				triViewed[2] = cameraMatrix.Multiply(triTransformed[2]);


				clipped.clear();
				ClipAgainsPlane(Vector3d(0, 0, scene.Cam.Near), Vector3d(0, 0, 1), IndependendFace(triViewed[0], triViewed[1], triViewed[2]), clipped);

				for (IndependendFace clippedFace : clipped) {

					triProjected[0] = projectionMatrix.Multiply(clippedFace.A);
					triProjected[1] = projectionMatrix.Multiply(clippedFace.B);
					triProjected[2] = projectionMatrix.Multiply(clippedFace.C);

					triProjected[0].Divide(triProjected[0].w);
					triProjected[1].Divide(triProjected[1].w);
					triProjected[2].Divide(triProjected[2].w);

					triProjected[0].y *= -1.0;
					triProjected[1].y *= -1.0;
					triProjected[2].y *= -1.0;

					triProjected[0].Transform(1, 1, 0);
					triProjected[1].Transform(1, 1, 0);
					triProjected[2].Transform(1, 1, 0);

					//###################

					triProjected[0].x *= width;
					triProjected[0].y *= height;

					triProjected[1].x *= width;
					triProjected[1].y *= height;

					triProjected[2].x *= width;
					triProjected[2].y *= height;

					//###################

					if ( ((triProjected[0].z + triProjected[1].z + triProjected[2].z) / 3.0f ) > scene.Cam.Near) {
						//Debug
						drawBuffor.push_back(IndependendFace(triProjected[0], triProjected[1], triProjected[2]));
					}

				} // Foreach clipped

			} // if dot product

		} //Foreach Face

	} // Foreach Meshes

	std::sort(std::begin(drawBuffor), std::end(drawBuffor), [](IndependendFace faceA, IndependendFace faceB) {
		
		float z1 = faceA.AverageZ();
		float z2 = faceB.AverageZ();

		if (z1 < z2) return 1;
		if (z1 == z2) return 0; else return -1;
		} );

	for (IndependendFace toDraw : drawBuffor) {
		drawTriangleFast(toDraw, img);
	}
	
}

Renderer::Renderer()
{
	this->frameRate = 30;
	this->showEdges = true;
	this->showFaces = true;
}

Renderer::Renderer(const Renderer& other)
{
	this->frameRate = other.frameRate;
	this->showEdges = other.showEdges;
	this->showFaces = other.showFaces;
}

Renderer::~Renderer()
{
}
