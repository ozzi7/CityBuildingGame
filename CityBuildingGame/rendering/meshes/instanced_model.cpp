#include "stdafx.h"
#include "stb_image.h"
#include "instanced_model.h"

InstancedModel::InstancedModel() {}

InstancedModel::InstancedModel(const std::string& path) 
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void InstancedModel::processNode(aiNode *node, const aiScene *scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;		
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];

		processMesh(aimesh, scene, &vertices, &indices, &textures);
		InstancedMesh mesh = InstancedMesh(vertices, indices, textures);
		meshes.push_back(mesh);
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


void InstancedModel::Draw(Shader &shader, std::vector<glm::mat4> &models)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Bind(shader, models);
		meshes[i].Draw((int)models.size());
	}
}
