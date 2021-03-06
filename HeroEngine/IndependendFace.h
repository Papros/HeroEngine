#pragma once
#include "Vector3d.h"
#include "Face.h"

class IndependendFace
{
public:
	Vector3d A, B, C;

	float AverageZ();

	IndependendFace();
	IndependendFace(const IndependendFace& other);
	IndependendFace(const Face &other);
	IndependendFace(Vector3d a, Vector3d b, Vector3d c);

	~IndependendFace();
};

