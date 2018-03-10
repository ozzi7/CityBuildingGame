#pragma once

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "tree.h"
#include "chamaecyparis.h"
#include "visitor.h"
#include "fir.h"
#include "terrain.h"
#include "shader.h"
#include "model.h"

class Renderer : public Visitor
{
public:
	Model *model_chamaecyparis;
	Model *model_fir;
	//Model *model_fir;
	Shader *shader_chamaecyparis;
	//Shader *shader_chamaecyparis;
	Shader *shader_fir;
	Shader *shader_terrain;

	glm::mat4 projection;
	glm::mat4 view;

	Renderer(std::string exe_path)
	{
		/* Chamaecyparis init*/
		shader_chamaecyparis = new Shader("mesh_shader.vert", "mesh_shader.frag");

		std::string texture_path = exe_path + "/../models/Chamaecyparis/Tree Chamaecyparis N161216.3ds";
		model_chamaecyparis = new Model(texture_path, false);

		/* fir init*/
		shader_fir = new Shader("mesh_shader.vert", "mesh_shader.frag");

		texture_path = exe_path + "/../models/fir/Fir.3DS";
		model_fir = new Model(texture_path, false);

		shader_terrain = new Shader("terrain.vert", "terrain.frag");
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		projection = aProjection;
		view = aView;
	}
	void Visit(Tree *tree)
	{
		// no general tree..
	};
	void Visit(Chamaecyparis *chamaecyparis)
	{
		/*shader_chamaecyparis->use();
		shader_chamaecyparis->setMat4("projection", projection);
		shader_chamaecyparis->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, chamaecyparis->position);
		model = glm::rotate(model, 90.0f, glm::vec3(1,0,0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02));
		model = glm::scale(model, chamaecyparis->scale);
		model = glm::rotate(model, chamaecyparis->rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		shader_chamaecyparis->setMat4("model", model);

		model_chamaecyparis->Draw(*shader_chamaecyparis);*/
	};
	void Visit(Fir *fir)
	{
		shader_fir->use();
		shader_fir->setMat4("projection", projection);
		shader_fir->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, fir->position);
		model = glm::translate(model, glm::vec3(0.0f,0.0f,0.3f));

		model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
		model = glm::scale(model, fir->scale);
		model = glm::rotate(model, fir->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		shader_fir->setMat4("model", model);

		model_fir->Draw(*shader_fir);
	};
	void Visit(Terrain *terrain)
	{
		shader_terrain->use();
		shader_terrain->setMat4("projection", projection);
		shader_terrain->setMat4("view", view);

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
		delete shader_chamaecyparis;
		delete shader_fir;
		delete shader_terrain;
	}
};