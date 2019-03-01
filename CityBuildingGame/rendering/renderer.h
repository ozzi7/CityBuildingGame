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
#include "fir.h"
#include "lumberjack.h"
#include "grass.h"
#include "triple_buffer.h"

class Renderer
{
public:
	//Model *model_fir;

	SkinnedMesh *mesh_lumberjack;
	InstancedModel *instanced_model_fir;
	InstancedModel *instanced_model_grass;
	Shader *terrain_shader;
	Shader *mesh_shader;
	Shader *skinned_mesh_shader;
	Shader *instanced_mesh_shader;
	//Shader *shadow_map_shader;

	float z = 0.0f;

	Renderer()
	{
		std::string root_path = Path;
		std::string texture_path;
		std::replace(root_path.begin(), root_path.end(), '\\', '/');

		terrain_shader = new Shader("shaders/terrain.vert", "shaders/terrain.frag");
		mesh_shader = new Shader("shaders/mesh_shader.vert", "shaders/mesh_shader.frag");
		skinned_mesh_shader = new Shader("shaders/skinning.vert", "shaders/skinning.frag");
		instanced_mesh_shader = new Shader("shaders/mesh_instanced.vert", "shaders/mesh_instanced.frag");
		//shadow_map_shader = new Shader("shadow_mapping_depth.vert", "shadow_mapping_depth.frag");

		/* fir init*/
		texture_path = root_path + "/../models/fir3/fir2.dae";
		instanced_model_fir = new InstancedModel(texture_path, false);

		/* Grass init*/
		texture_path = root_path + "/../models/grass/test.dae";
		instanced_model_grass = new InstancedModel(texture_path, false);

		/* lumberjack init*/
		//texture_path = root_path + "/../models/zombie/Zombie.fbx";
		mesh_lumberjack = new SkinnedMesh();
		//texture_path = root_path + "/../models/minotaur/animation/Minotaur@Walk.dae";
		//texture_path = root_path + "/../models/mario/test_walk.dae";
		texture_path = root_path + "/../models/lumberjack/lumberjack.dae";
		mesh_lumberjack->LoadMesh(texture_path);
		mesh_lumberjack->PrecalculateBoneTransforms();	
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		mesh_shader->use();
		mesh_shader->setMat4("projection", aProjection);
		mesh_shader->setMat4("view", aView);

		terrain_shader->use();
		terrain_shader->setMat4("projection", aProjection);
		terrain_shader->setMat4("view", aView);

		skinned_mesh_shader->use();
		skinned_mesh_shader->setMat4("projection", aProjection);
		skinned_mesh_shader->setMat4("view", aView);

		instanced_mesh_shader->use();
		instanced_mesh_shader->setMat4("projection", aProjection);
		instanced_mesh_shader->setMat4("view", aView);
	}
	void OpenGLStart() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glDepthMask(TRUE);
	}
	void Render(TripleBuffer *renderBuffer)
	{
		RenderInstancedObjects(renderBuffer);
	}
	void Visit(Tree *tree) {};
	/*void Visit(Fir *fir)
	{
		dataFir.models.push_back(fir->model);
	};
	void Visit(Grass *grass)
	{
		dataGrass.models.push_back(grass->model);
	}*/
	void Visit(Lumberjack *lumberjack)
	{
		skinned_mesh_shader->use();
		z = z + 0.0011f; // TODO: speed of animation doesnt belong here...
		mesh_lumberjack->BindBoneTransform(z, skinned_mesh_shader);
		skinned_mesh_shader->setMat4("model", lumberjack->model);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mesh_lumberjack->Render(*skinned_mesh_shader);
	}
	void Visit(Terrain *terrain)
	{
		terrain_shader->use();

		glm::vec3 lightColor;
		lightColor.x = 1.0f;
		lightColor.y = 1.0f;
		lightColor.z = 1.0f;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence
		terrain_shader->setVec3("light.ambient", ambientColor);
		terrain_shader->setVec3("light.diffuse", diffuseColor);
		//terrain_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//terrain_shader->setVec3("light.position", glm::vec3(100.0f, 100.0f, 40.0f));//camera->Position);
		terrain_shader->setVec3("light.direction", glm::vec3(-1.0, -1.0, -1.0));
		terrain_shader->setVec3("viewPos", glm::vec3(10.0f, 10.0f, 10.0f));

		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		terrain_shader->setMat4("model", model);

		terrain->Draw();
	}
	void RenderInstancedObjects(TripleBuffer *renderBuffer)
	{
		glDepthMask(FALSE);
		/*Use of continuous alpha values requires blending*/
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		instanced_mesh_shader->use();
		instanced_model_grass->Draw(*instanced_mesh_shader, renderBuffer->grassModels); // note shader.use() is in model

		/*set the light source*/
		glm::vec3 lightColor;
		lightColor.x = 1.0f;
		lightColor.y = 1.0f;
		lightColor.z = 1.0f;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence
		instanced_mesh_shader->setVec3("light.ambient", ambientColor);
		instanced_mesh_shader->setVec3("light.diffuse", diffuseColor);
		//terrain_shader->setVec3("light.position", glm::vec3(100.0f, 100.0f, 40.0f));//camera->Position);
		instanced_mesh_shader->setVec3("light.direction", glm::vec3(-1.0, -1.0, -1.0));
		instanced_mesh_shader->setVec3("viewPos", glm::vec3(10.0f, 10.0f, 10.0f));

		/*draw instanced objects*/
		instanced_model_fir->Draw(*instanced_mesh_shader, renderBuffer->firModels); // note shader.use() is in model
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