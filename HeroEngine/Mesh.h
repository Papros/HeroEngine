#pragma once

#include <string>
#include <vector>
#include "Vector3d.h"
#include "Face.h"

class Mesh
{


public:
	std::string Name;
	std::vector<Vector3d> Vertices;
	std::vector<Face> Faces;
	Vector3d Position;
	Vector3d Rotation;

	std::string GetName() const;
	bool SetName(std::string name);
	bool AddVertices(Vector3d vector);
	bool AddFace(int a, int b, int c);
	bool SetPosition(Vector3d position);
	bool SetRotation(Vector3d rotation);

	Mesh();
	Mesh(const Mesh& other);
	~Mesh();
};

