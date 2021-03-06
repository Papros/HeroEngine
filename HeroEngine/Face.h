#pragma once

#include "Vector3d.h"

class Face {

public:
	Vector3d  *A, *B, *C;
	Face();
	Face(const Face& other);
	Face(Vector3d &a, Vector3d &b, Vector3d &c);
	~Face();


};

