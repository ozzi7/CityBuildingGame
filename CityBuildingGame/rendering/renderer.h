#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "globals.h"
#include "terrain.h"
#include "shader.h"
#include "model.h"
#include "skinned_mesh.h"
#include "instanced_model.h"
#include "shadow.h"

#include "tree.h"
#include "lumberjack.h"
#include "triple_buffer.h"

struct LightSource
{
	glm::vec3 Direction;
	glm::vec3 Color;
};

class Renderer
{
public:
	SkinnedMesh* mesh_lumberjack;
	Shader* terrain_shader;
	Shader* skinned_mesh_shader;
	Shader* instanced_mesh_shader;
	Shader* shadow_shader;
	Shader* shadow_instanced_shader;
	Shader* shadow_skinned_shader;
	InstancedModel* instanced_model_pine;
	InstancedModel* instanced_model_juniper;
	InstancedModel* instanced_model_spruce;
	InstancedModel* instanced_model_oak;

	LightSource directionalLight;
	glm::vec3 ambientLight;

	bool ShadowPass = false;
	float z = 0.0f;

	Renderer()
	{
		std::string model_path;

		terrain_shader = new Shader("shaders/mesh.vert", "shaders/mesh.frag");
		skinned_mesh_shader = new Shader("shaders/mesh_skinned.vert", "shaders/mesh.frag");
		instanced_mesh_shader = new Shader("shaders/mesh_instanced.vert", "shaders/mesh.frag");

		shadow_shader = new Shader("shaders/shadow_depth.vert", "shaders/shadow_depth.frag");
		shadow_instanced_shader = new Shader("shaders/shadow_depth_instanced.vert", "shaders/shadow_depth.frag");
		shadow_skinned_shader = new Shader("shaders/shadow_depth_skinned.vert", "shaders/shadow_depth.frag");

		/* vegetation*/
		model_path = Path + "/../models/pine/pine.dae";
		instanced_model_pine = new InstancedModel(model_path);

		model_path = Path + "/../models/juniper/juniper.dae";
		instanced_model_juniper = new InstancedModel(model_path);

		model_path = Path + "/../models/spruce/spruce.dae";
		instanced_model_spruce = new InstancedModel(model_path);

		model_path = Path + "/../models/oak/oak.dae";
		instanced_model_oak = new InstancedModel(model_path);

		/* lumberjack init*/
		model_path = Path + "/../models/lumberjack/lumberjack.dae";
		mesh_lumberjack = new SkinnedMesh();
		mesh_lumberjack->LoadMesh(model_path);
		mesh_lumberjack->PrecalculateBoneTransforms();

		directionalLight.Color = { 1.0f, 1.0f, 1.0f };
		directionalLight.Direction = { -1.0f, -1.0f, -1.0f };

		ambientLight = { 0.5f, 0.5f, 0.5f };
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView, glm::mat4 aLightSpaceMatrix)
	{
		if (ShadowPass)
		{
			shadow_shader->use();
			shadow_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);

			shadow_instanced_shader->use();
			shadow_instanced_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);

			shadow_skinned_shader->use();
			shadow_skinned_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
		}
		else
		{
			terrain_shader->use();
			terrain_shader->setMat4("projection", aProjection);
			terrain_shader->setMat4("view", aView);
			terrain_shader->setMat4("model", glm::mat4(1.0f));
			terrain_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			terrain_shader->setInt("shadowMap", 13);

			skinned_mesh_shader->use();
			skinned_mesh_shader->setMat4("projection", aProjection);
			skinned_mesh_shader->setMat4("view", aView);
			skinned_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			skinned_mesh_shader->setInt("shadowMap", 13);

			instanced_mesh_shader->use();
			instanced_mesh_shader->setMat4("projection", aProjection);
			instanced_mesh_shader->setMat4("view", aView);
			instanced_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			instanced_mesh_shader->setInt("shadowMap", 13);
		}
	}
	void Render(RenderBuffer* renderBuffer)
	{
		RenderTerrain(renderBuffer);
		RenderInstancedObjects(renderBuffer);
		RenderBoneAnimated(renderBuffer);
	}
	void RenderTerrain(RenderBuffer* renderBuffer)
	{
		if (!ShadowPass) { // don't calculate terrain shadows
			terrain_shader->use();
			terrain_shader->setVec3("light.ambient", ambientLight);
			terrain_shader->setVec3("light.diffuse", directionalLight.Color);
			terrain_shader->setVec3("light.direction", directionalLight.Direction);
			renderBuffer->terrain->Draw();
		}
	}
	void RenderInstancedObjects(RenderBuffer* renderBuffer)
	{
		Shader* shader;
		if (ShadowPass)
		{
			shader = shadow_instanced_shader;
			shader->use();
		}
		else
		{
			shader = instanced_mesh_shader;
			shader->use();
			shader->setVec3("light.ambient", ambientLight);
			shader->setVec3("light.diffuse", directionalLight.Color);
			shader->setVec3("light.direction", directionalLight.Direction);
		}
		instanced_model_pine->Draw(*shader, renderBuffer->pineModels);
		instanced_model_oak->Draw(*shader, renderBuffer->oakModels);
		instanced_model_spruce->Draw(*shader, renderBuffer->spruceModels);
		instanced_model_juniper->Draw(*shader, renderBuffer->juniperModels);
	}
	void RenderBoneAnimated(RenderBuffer* renderBuffer)
	{
		Shader* shader;
		if (ShadowPass)
		{
			shader = shadow_skinned_shader;
			shader->use();
		}
		else
		{
			shader = skinned_mesh_shader;
			shader->use();
			shader->setVec3("light.ambient", ambientLight);
			shader->setVec3("light.diffuse", directionalLight.Color);
			shader->setVec3("light.direction", directionalLight.Direction);
		}
		z = z + 0.0011f; // TODO: speed of animation doesnt belong here...
		mesh_lumberjack->BindBoneTransform(z, shader);

		for (int i = 0; i < renderBuffer->lumberjackModels.size(); ++i) {
			shader->setMat4("model", renderBuffer->lumberjackModels[i]);
			mesh_lumberjack->Render(*shader);
		}
	}

	~Renderer()
	{
		//delete model_fir;
		//delete mesh_lumberjack;
		//delete terrain_shader;
	}
};