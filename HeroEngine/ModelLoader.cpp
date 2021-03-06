#include "ModelLoader.h"
#include <iostream>

void printModel(Mesh& mesh) {

	std::cout << mesh.Name <<std::endl;

	for (Vector3d vec : mesh.Vertices) {
		std::cout << vec.ToString()<<std::endl;
	}

	for (Face fac : mesh.Faces) {
		std::cout<< fac.A->ToString()<<":"<<fac.B->ToString() <<":"<<fac.C->ToString() <<std::endl;
	}
}

bool LoadOBJFile(std::string path, Mesh& mesh) {

	//Load from file

	std::ifstream model(path);

	std::string symbol;
	float x, y, z;
	int xi, yi, zi;
	std::string name, a, b, c;

	for (std::string line; getline(model, line); )
	{
		std::istringstream input(line);
		
		if (!(input >> symbol)) break;

		if (symbol._Equal("v")) {
			if (input >> x >> y >> z) mesh.AddVertices(Vector3d(x, y, z));
		}
		else if (symbol._Equal("f")) {
			if (input >> xi >> a >> yi >> b >> zi >> c)
				mesh.AddFace(xi-1, yi-1, zi-1);
		}
		else if (symbol._Equal("o")) {
			if (input >> name) mesh.SetName(name);
		}

	}

	return true;
}

bool LoadDAEFile(std::string path, Mesh& mesh) {

	std::vector<Vector3d> vertex_position;
	std::vector<Face> mesh_faces;

	std::ifstream model(path);

	float x,y,z;
	int a, b, c, unused;
	std::string tag;
	std::string pattern1 = "<geometry"; bool pat1 = false;
	std::string pattern2 = "<float_array"; bool pat2 = false;
	std::string pattern3 = "<triangles"; bool pat3 = false;

	for (std::string line; getline(model, line); )
	{
		std::istringstream input(line);
		
		if (!pat1) {

			input >> tag;
			if (tag == pattern1) {
				input >> tag;
				input >> tag;


				mesh.Name = tag.substr(6, tag.length() - 6 - 2);
				pat1 = true;
			}
		}
		else if(!pat2)
		{
			input >> tag;
			if (tag == pattern2) {
				std::cout << line << std::endl;
				line = line.substr(2, line.length() - 2);
				std::cout << line << std::endl;
				int n = line.find(">");
				line = line.substr(n+1, line.length() - n - 14);
				std::cout << line << std::endl;

				std::istringstream vertexStr(line);
				
				while (vertexStr >> x >> y >> z) {
					mesh.AddVertices(Vector3d(x, y, z));
				}
				pat2 = true;
			}

		}
		else if(!pat3){

			input >> tag;
			if (tag == pattern3) {
				getline(model, line);
				getline(model, line);
				getline(model, line);
				getline(model, line);

				line = line.substr(2, line.length() - 4);
				int n = line.find(">");
				line = line.substr(n+1, line.length() - n - 4);
				std::istringstream vertexStr(line);

				std::cout << ":> " << line << std::endl;
				while (vertexStr >> a >> unused >> unused >> b >> unused >> unused >> c >> unused >> unused) {
					mesh.AddFace(a,b,c);
				}

				pat3 = true;
			}

		}

		if (pat1 && pat2 && pat3) {
			std::cout << "Model loaded properly: " << path << std::endl;
			return true;
		}

	}
	std::cout << "Error when parsing: " << path << std::endl;
	return false;
}


bool ModelLoader::LoadModel(std::string path, Mesh& mesh, FileType type)
{
	
	switch (type) {
		case FileType::OBJ: return LoadOBJFile(path, mesh); break;
		case FileType::DAE: return LoadDAEFile(path, mesh); break;
		default: return LoadOBJFile(path, mesh);
	}

	return false;
}


