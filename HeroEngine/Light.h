#pragma once
#include "Vector3d.h"

class Light
{
	public:
		Vector3d position;

		Light();
		Light(const Light& other);
		~Light();

};

