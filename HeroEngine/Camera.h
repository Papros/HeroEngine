#pragma once
#include "Vector3d.h"


class Camera
{
	public:
		Vector3d position;
		Vector3d lookDir;
		Vector3d upDirection;

		float angle, Yaw, Theta, Far, Near;

		Camera();
		Camera(const Camera& other);
		~Camera();
};

