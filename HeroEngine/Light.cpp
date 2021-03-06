#include "Light.h"

Light::Light()
{
	this->position = Vector3d(0, 0, 0);
}

Light::Light(const Light& other)
{
	this->position = Vector3d(other.position);
}

Light::~Light()
{
}
