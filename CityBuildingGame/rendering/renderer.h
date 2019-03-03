#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "globals.h"
#include "terrain.h"
#include "shader.h"
#include "model.h"
#include "skinned_mesh.hpp"
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
	InstancedModel *instanced_model_pine;
	InstancedModel *instanced_model_juniper;
	InstancedModel *instanced_model_spruce;
	InstancedModel *instanced_model_oak;

	LightSource directionalLight;
	glm::vec3 ambientLight;

	bool ShadowPass = false;
	float z = 0.0f;

	Renderer()
	{
		std::string root_path = Path;
		std::string texture_path;
		std::replace(root_path.begin(), root_path.end(), '\\', '/');

		terrain_shader = new Shader("shaders/terrain.vert", "shaders/mesh.frag");
		skinned_mesh_shader = new Shader("shaders/skinning.vert", "shaders/skinning.frag");
		instanced_mesh_shader = new Shader("shaders/mesh.vert", "shaders/mesh.frag");

		shadow_shader = new Shader("shaders/shadow_depth.vert", "shaders/shadow_depth.frag");

		/* vegetation*/
		texture_path = root_path + "/../models/pine/pine.dae";
		instanced_model_pine = new InstancedModel(texture_path);

		texture_path = root_path + "/../models/juniper/juniper.dae";
		instanced_model_juniper = new InstancedModel(texture_path);

		texture_path = root_path + "/../models/spruce/spruce.dae";
		instanced_model_spruce = new InstancedModel(texture_path);

		texture_path = root_path + "/../models/oak/oak.dae";
		instanced_model_oak = new InstancedModel(texture_path);

		/* lumberjack init*/
		mesh_lumberjack = new SkinnedMesh();
		texture_path = root_path + "/../models/lumberjack/lumberjack_new.dae";
		mesh_lumberjack = new SkinnedMesh();
		mesh_lumberjack->LoadMesh(texture_path);
		mesh_lumberjack->PrecalculateBoneTransforms();

		directionalLight.Color = { 0.8f, 0.8f, 0.8f };
		directionalLight.Direction = { -1.0f, -1.0f, -1.0f };

		ambientLight = { 0.2f, 0.2f, 0.2f };
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView, glm::mat4 aLightSpaceMatrix)
	{
		if (ShadowPass)
		{
			shadow_shader->use();
			shadow_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
		}
		else
		{
			terrain_shader->use();
			terrain_shader->setMat4("projection", aProjection);
			terrain_shader->setMat4("view", aView);
			terrain_shader->setMat4("model", glm::mat4(1.0f));
			terrain_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			terrain_shader->setInt("shadowMap", 1);

			skinned_mesh_shader->use();
			skinned_mesh_shader->setMat4("projection", aProjection);
			skinned_mesh_shader->setMat4("view", aView);
			skinned_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			skinned_mesh_shader->setInt("shadowMap", 1);

			instanced_mesh_shader->use();
			instanced_mesh_shader->setMat4("projection", aProjection);
			instanced_mesh_shader->setMat4("view", aView);
			instanced_mesh_shader->setMat4("lightSpaceMatrix", aLightSpaceMatrix);
			instanced_mesh_shader->setInt("shadowMap", 1);
		}
	}
	void OpenGLStart() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(TRUE);
	}
	void Render(RenderBuffer* renderBuffer)
	{
		RenderTerrain(renderBuffer);
		RenderBoneAnimated(renderBuffer);
		RenderInstancedObjects(renderBuffer);
	}
	void RenderTerrain(RenderBuffer* renderBuffer)
	{
		Shader* shader;
		if (ShadowPass) 
		{
			shader = shadow_shader;
			shader->use();
		}
		else
		{
			shader = terrain_shader;
			shader->use();
			shader->setVec3("light.ambient", ambientLight);
			shader->setVec3("light.diffuse", directionalLight.Color);
			shader->setVec3("light.direction", directionalLight.Direction);
		}

		renderBuffer->terrain->Draw();
	}
	void RenderInstancedObjects(RenderBuffer* renderBuffer)
	{
		Shader* shader;
		if (ShadowPass)
		{
			shader = shadow_shader;
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

		/*draw instanced objects*/
		instanced_model_pine->Draw(*shader, renderBuffer->pineModels);
		instanced_model_oak->Draw(*shader, renderBuffer->oakModels);
		instanced_model_spruce->Draw(*shader, renderBuffer->spruceModels);
		instanced_model_juniper->Draw(*shader, renderBuffer->juniperModels);
	}
	void RenderBoneAnimated(RenderBuffer* renderBuffer)
	{
		skinned_mesh_shader->use();
		z = z + 0.0011f; // TODO: speed of animation doesnt belong here...
		mesh_lumberjack->BindBoneTransform(z, skinned_mesh_shader);

		for (int i = 0; i < renderBuffer->lumberjackModels.size(); ++i) {
			skinned_mesh_shader->setMat4("model", renderBuffer->lumberjackModels[i]);
			mesh_lumberjack->Render(*skinned_mesh_shader);
		}
	}

	~Renderer()
	{
		//delete model_fir;
		//delete mesh_lumberjack;
		//delete terrain_shader;
	}
};