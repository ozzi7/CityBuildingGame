#include "stdafx.h"
#include "instanced_mesh.h"

InstancedMesh::InstancedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
                             std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
	setupInstancedMesh();
}

void InstancedMesh::setupInstancedMesh()
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &instanceVBO);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * textures.size(), textures.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)nullptr);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)sizeof(glm::vec4));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void InstancedMesh::Draw(int instances) const
{
	glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr, instances);

	// unbind
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void InstancedMesh::Bind(Shader& shader, std::vector<glm::mat4>& models)
{
	Mesh::Bind(shader);

	glBindVertexArray(VAO);

	// bind per instance data
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * models.size(), models.data(), GL_STATIC_DRAW);
}
