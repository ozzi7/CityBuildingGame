#include "stdafx.h"
#include "stb_image.h"
#include "instanced_model.h"

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::string& path) 
{
	init(path);
}

void InstancedModel::addMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	InstancedMesh mesh = InstancedMesh(vertices, indices, textures);
	meshes.push_back(mesh);
}


void InstancedModel::Draw(Shader &shader, std::vector<glm::mat4> &models)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Bind(shader, models);
		meshes[i].Draw((int)models.size());
	}
}
