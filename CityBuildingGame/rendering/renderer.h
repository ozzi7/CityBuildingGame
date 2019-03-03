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
	InstancedModel *instanced_model_pine;
	InstancedModel *instanced_model_juniper;
	InstancedModel *instanced_model_spruce;
	InstancedModel *instanced_model_oak;

	LightSource directionalLight;
	glm::vec3 ambientLight;

	float z = 0.0f;

	Renderer()
	{
		std::string root_path = Path;
		std::string texture_path;
		std::replace(root_path.begin(), root_path.end(), '\\', '/');

		terrain_shader = new Shader("shaders/terrain.vert", "shaders/mesh.frag");
		skinned_mesh_shader = new Shader("shaders/skinning.vert", "shaders/skinning.frag");
		instanced_mesh_shader = new Shader("shaders/mesh.vert", "shaders/mesh.frag");

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
		texture_path = root_path + "/../models/lumberjack/lumberjack.dae";
		mesh_lumberjack = new SkinnedMesh();
		mesh_lumberjack->LoadMesh(texture_path);
		mesh_lumberjack->PrecalculateBoneTransforms();

		directionalLight.Color = { 0.8f, 0.8f, 0.8f };
		directionalLight.Direction = { -1.0f, -1.0f, -1.0f };

		ambientLight = { 0.3f, 0.3f, 0.3f };
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		terrain_shader->use();
		terrain_shader->setMat4("projection", aProjection);
		terrain_shader->setMat4("view", aView);
		terrain_shader->setMat4("model", glm::mat4(1.0f));

		skinned_mesh_shader->use();
		skinned_mesh_shader->setMat4("projection", aProjection);
		skinned_mesh_shader->setMat4("view", aView);

		instanced_mesh_shader->use();
		instanced_mesh_shader->setMat4("projection", aProjection);
		instanced_mesh_shader->setMat4("view", aView);
	}
	void OpenGLStart() {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
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
		terrain_shader->use();
		terrain_shader->setVec3("light.ambient", ambientLight);
		terrain_shader->setVec3("light.diffuse", directionalLight.Color);
		terrain_shader->setVec3("light.direction", directionalLight.Direction);

		renderBuffer->terrain->Draw();
	}
	void RenderInstancedObjects(RenderBuffer* renderBuffer)
	{
		//glDepthMask(FALSE);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		instanced_mesh_shader->use();
		instanced_mesh_shader->setVec3("light.ambient", ambientLight);
		instanced_mesh_shader->setVec3("light.diffuse", directionalLight.Color);
		instanced_mesh_shader->setVec3("light.direction", directionalLight.Direction);

		/*draw instanced objects*/
		instanced_model_pine->Draw(*instanced_mesh_shader, renderBuffer->pineModels); // note shader.use() is in model
		instanced_model_oak->Draw(*instanced_mesh_shader, renderBuffer->oakModels); // note shader.use() is in model
		instanced_model_spruce->Draw(*instanced_mesh_shader, renderBuffer->spruceModels); // note shader.use() is in model
		instanced_model_juniper->Draw(*instanced_mesh_shader, renderBuffer->juniperModels); // note shader.use() is in model
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
	void RenderDepthMap()
	{
		//// 1. render depth of scene to texture (from light's perspective)
		//// --------------------------------------------------------------
		//glm::mat4 lightProjection, lightView;
		//glm::mat4 lightSpaceMatrix;
		//float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		//lightSpaceMatrix = lightProjection * lightView;

		//// render scene from light's point of view
		//shadow_map_shader->use();
		//shadow_map_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//glViewport(0, 0, 1024, 1024);
		//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		//renderScene(simpleDepthShader);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//// reset viewport
		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	~Renderer()
	{
		//delete model_fir;
		//delete mesh_lumberjack;
		//delete terrain_shader;
	}
};