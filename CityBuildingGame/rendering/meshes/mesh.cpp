#include "stdafx.h"
#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Bind(Shader& shader)
{
	unsigned int diffuseNr = 0;
	shader.setBool("hasSpecular", false);
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// retrieve texture number (the N in diffuse_textureN)
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			glActiveTexture(GL_TEXTURE0 + diffuseNr);
			diffuseNr++;
			glUniform1i(glGetUniformLocation(shader.ID, (name + std::to_string(diffuseNr)).c_str()), i);
		}
		else if (name == "texture_specular")
		{
			glActiveTexture(GL_TEXTURE10);
			glUniform1i(glGetUniformLocation(shader.ID, (name + "0").c_str()), i);
			shader.setBool("hasSpecular", true);
		}
		else if (name == "texture_normal")
		{
			glActiveTexture(GL_TEXTURE11);
			glUniform1i(glGetUniformLocation(shader.ID, (name + "0").c_str()), i);
		}
		else if (name == "texture_height")
		{
			glActiveTexture(GL_TEXTURE12);
			glUniform1i(glGetUniformLocation(shader.ID, (name + "0").c_str()), i);
		}
		
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

void Mesh::Draw() const
{
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

	// unbind
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
