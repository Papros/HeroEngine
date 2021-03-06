#include "IndependendFace.h"

IndependendFace::IndependendFace(const IndependendFace& other) {
	this->A = Vector3d(other.A);
	this->B = Vector3d(other.B);
	this->C = Vector3d(other.C);
}

IndependendFace::IndependendFace(const Face &other) {
	this->A = Vector3d(*other.A);
	this->B = Vector3d(*other.B);
	this->C = Vector3d(*other.C);
}

IndependendFace::IndependendFace(Vector3d a, Vector3d b, Vector3d c)
{
	this->A = a;
	this->B = b;
	this->C = c;
}

float IndependendFace::AverageZ()
{
	return (A.z + B.z + C.z) / 3;
}

IndependendFace::IndependendFace()
{
	this->A = Vector3d();
	this->B = Vector3d();
	this->C = Vector3d();
}

IndependendFace::~IndependendFace()
{

}
