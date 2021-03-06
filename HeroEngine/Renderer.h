#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "TransformMatrix.h"
#include "IndependendFace.h"



class Renderer
{
	public:
		int frameRate;
		bool showFaces, showEdges;

		void Render(const Scene &scene, sf::RenderTarget &img);

		Renderer();
		Renderer(const Renderer& other);
		~Renderer();

};

