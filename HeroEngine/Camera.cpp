#include "Camera.h"

Camera::Camera()
{
	this->position = Vector3d(0.0f, 0.0f, 10.0f);
	this->lookDir = Vector3d(0.0f, 0.0f, -1.0f);
	this->upDirection = Vector3d(0.0f, 1.0f, 0.0f);
	angle = 0.5f; Yaw = 0.0f; Theta = 0.0f; Far = 1000.0f; Near=0.1f;
}

Camera::Camera(const Camera& other)
{
	this->position = Vector3d(other.position);
	this->lookDir = Vector3d(other.lookDir);
	this->upDirection = Vector3d(other.upDirection);
	angle = other.angle; Yaw = other.Yaw; Theta = other.Theta; Far = other.Far; Near = other.Near;
}

Camera::~Camera()
{
}
