#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "globals.h"
#include "mesh.h"
#include "shader.h"
#include "mipmap_generator.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
	// constructor, expects a filepath to a 3D model.
	Model();
	Model(const std::string& path); // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.

	// draws the model, and thus all its meshes
	void Draw(Shader& shader);
	unsigned int TextureFromFile(std::string& path);

protected:
	void init(const std::string& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	void virtual addMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	//  vertices, indicies, textures are return values
	void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture>* textures);

	std::string directory;

	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded;

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName);

	std::vector<Mesh> meshes;
};
