#pragma once

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "visitor.h"
#include "terrain.h"
#include "shader.h"
#include "model.h"
#include "tree.h"
#include "chamaecyparis.h"
#include "fir.h"
#include "palm.h"

class Renderer : public Visitor
{
public:
	Model *model_chamaecyparis;
	Model *model_fir;

	Shader *shader_terrain;
	Model *model_palm;
	Shader *mesh_shader;

	Renderer(std::string exe_path)
	{
		std::string texture_path;
		std::replace(exe_path.begin(), exe_path.end(), '\\', '/');

		mesh_shader = new Shader("mesh_shader.vert", "mesh_shader.frag");

		/* Chamaecyparis init*/
		texture_path = exe_path + "/../models/Chamaecyparis/Tree Chamaecyparis N161216.3ds";
		model_chamaecyparis = new Model(texture_path, false);

		/* fir init*/
		texture_path = exe_path + "/../models/fir2/fir.obj";
		model_fir = new Model(texture_path, false);

		shader_terrain = new Shader("terrain.vert", "terrain.frag");

		/* Palm init*/
		texture_path = exe_path + "/../models/palm/palm1.obj";
		model_palm = new Model(texture_path, false);
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		mesh_shader->use();
		mesh_shader->setMat4("projection", aProjection);
		mesh_shader->setMat4("view", aView);

		shader_terrain->use();
		shader_terrain->setMat4("projection", aProjection);
		shader_terrain->setMat4("view", aView);
	}
	void Visit(Tree *tree) {};
	void Visit(Chamaecyparis *chamaecyparis)
	{
		mesh_shader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, chamaecyparis->position);
		model = glm::rotate(model, 90.0f, glm::vec3(1,0,0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02));
		model = glm::scale(model, chamaecyparis->scale);
		model = glm::rotate(model, chamaecyparis->rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		mesh_shader->setMat4("model", model);

		model_chamaecyparis->Draw(*mesh_shader);
	};
	void Visit(Fir *fir)
	{
		mesh_shader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, fir->position);
		//model = glm::translate(model, glm::vec3(0.0f,0.0f,0.3f));

		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::scale(model, fir->scale);
		model = glm::rotate(model, fir->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		mesh_shader->setMat4("model", model);

		model_fir->Draw(*mesh_shader);
	};
	void Visit(Palm *palm)
	{
		mesh_shader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, palm->position);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05));

		mesh_shader->setMat4("model", model);

		model_palm->Draw(*mesh_shader);
	};
	void Visit(Terrain *terrain)
	{
		shader_terrain->use();

		glm::vec3 lightColor;
		lightColor.x = 1.0f;//sin(glfwGetTime() * 2.0f);
		lightColor.y = 1.0f;// sin(glfwGetTime() * 0.7f);
		lightColor.z = 1.0f;// sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		shader_terrain->setVec3("light.ambient", ambientColor);
		shader_terrain->setVec3("light.diffuse", diffuseColor);
		shader_terrain->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		shader_terrain->setVec3("light.position", glm::vec3(10.0f, 10.0f, 10.0f));//camera->Position);
		shader_terrain->setVec3("viewPos", glm::vec3(10.0f, 10.0f, 10.0f));

		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		shader_terrain->setMat4("model", model);

		terrain->Draw(*shader_terrain);
	}
	~Renderer()
	{
		delete model_chamaecyparis;
		delete model_fir;
		delete shader_terrain;
	}
};