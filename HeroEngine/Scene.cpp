#include "Scene.h"

Scene::Scene()
{
	this->Cam = Camera();
	this->LightSources = std::vector<Light>();
	this->Objects = std::vector<Mesh>();
}

Scene::Scene(const Scene& other)
{
	this->Cam = Camera(other.Cam);
	this->LightSources = std::vector<Light>(other.LightSources);
	this->Objects = std::vector<Mesh>(other.Objects);
}

Scene::~Scene()
{
}
