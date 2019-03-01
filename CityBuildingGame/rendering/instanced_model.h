#pragma once

#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "model.h"
#include "instanced_mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class InstancedModel: public Model
{
public:
	// constructor, expects a filepath to a 3D model.	
	InstancedModel();
	InstancedModel(const std::string& path);

	void InstancedModel::Draw(Shader& shader, std::vector<glm::mat4>& models);
private:

	std::vector<InstancedMesh> meshes;
};