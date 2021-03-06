#include "Face.h"

Face::Face()
{
	this->A = 0;
	this->B = 0;
	this->C = 0;
}

Face::Face(const Face &other)
{
	this->A = other.A;
	this->B = other.B;
	this->C = other.C;
}

Face::Face(Vector3d& a, Vector3d& b, Vector3d& c)
{
	this->A = &a;
	this->B = &b;
	this->C = &c;
}

Face::~Face()
{
	// a,b,c is stored in vector in Mesh, no need to delete them here
	this->A = 0;
	this->B = 0;
	this->C = 0;
}