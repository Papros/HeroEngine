#include "Mesh.h"

std::string Mesh::GetName()
{
	return this->Name;
}

bool Mesh::SetName(std::string name)
{
	this -> Name = name;
	return true;
}

bool Mesh::AddVertices(Vector3d vector)
{
	this->Vertices.push_back(vector);
	return true;
}

bool Mesh::AddFace(int a, int b, int c)
{
	int size = this->Vertices.size();

	if (a < 0 || b < 0 || c < 0 || a == b || a == c || b == c || a > size || b > size || c > size) {
		std::cout << "Adding face error: vector = (0," << size << ") but faces are: [ " << a << " , " << b << " , " << c << " ]" << std::endl;
		return false;
	}

	this->Faces.push_back( Face(this->Vertices.at(a), this->Vertices.at(b), this->Vertices.at(c)) );
	return true;
}

bool Mesh::SetPosition(Vector3d position)
{
	this->Position = position;
	return true;
}

bool Mesh::SetRotation(Vector3d rotation)
{
	this->Rotation = rotation;
	return true;
}

Mesh::Mesh()
{
	this->Position = Vector3d(0.0,0.0,0.0,0.0);
	this->Rotation = Vector3d(0.0,0.0,0.0,0.0);
	this->Vertices = std::vector<Vector3d>();
	this->Faces = std::vector<Face>();
}

Mesh::Mesh(const Mesh& other)
{

	this->Position = Vector3d(other.Position);
	this->Rotation = Vector3d(other.Rotation);
	this->Vertices = std::vector<Vector3d>(other.Vertices);
	this->Faces = std::vector<Face>(other.Faces);
}

Mesh::~Mesh()
{
}
