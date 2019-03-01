#include "stdafx.h"
#include "stb_image.h"
#include "instanced_model.h"

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::string& path) {}

void InstancedModel::Draw(Shader &shader, std::vector<glm::mat4> &models)
{
	shader.use();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Bind(shader, models);
		meshes[i].Draw((int)models.size());
	}
}
