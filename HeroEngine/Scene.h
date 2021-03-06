#pragma once

#include <vector>

#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

class Scene
{
public:
	std::vector<Mesh> Objects;
	Camera Cam;
	std::vector<Light> LightSources;

	Scene();
	Scene(const Scene& other);
	~Scene();
};

