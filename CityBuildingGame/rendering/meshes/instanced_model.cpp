#include "stdafx.h"
#include "stb_image.h"
#include "instanced_model.h"

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::string& path)
{
	init(path);
}

void InstancedModel::addMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
                             std::vector<Texture> textures)
{
	InstancedMesh mesh = InstancedMesh(vertices, indices, textures);
	meshes.push_back(mesh);
}

void InstancedModel::Draw(Shader& shader, std::vector<glm::mat4>& models)
{
	for (InstancedMesh mesh : meshes)
	{
		mesh.Bind(shader, models);
		mesh.Draw((int)models.size());
	}
}
void InstancedModel::DrawOnly(Shader& shader, std::vector<glm::mat4>& models)
{
	for (InstancedMesh mesh : meshes)
	{
		mesh.BindOnly(shader);
		mesh.Draw((int)models.size());
	}
}
