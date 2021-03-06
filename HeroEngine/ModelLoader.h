#pragma once
#include <string>
#include "Mesh.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

class ModelLoader
{
public:

	enum class FileType {
		OBJ, DAE
	};

	bool LoadModel(std::string path, Mesh& mesh, FileType type);
};

