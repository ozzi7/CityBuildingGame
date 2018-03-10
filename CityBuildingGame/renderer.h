#pragma once

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "visitor.h"
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
	Model *model_palm;
	Shader *mesh_shader;

	//glm::mat4 projection;
	//glm::mat4 view;

	Renderer(std::string exe_path)
	{
		std::string texture_path;
		std::replace(exe_path.begin(), exe_path.end(), '\\', '/');

		mesh_shader = new Shader("mesh_shader.vert", "mesh_shader.frag");

		/* Chamaecyparis init*/
		texture_path = exe_path + "/../models/Chamaecyparis/Tree Chamaecyparis N161216.3ds";
		model_chamaecyparis = new Model(texture_path, false);

		/* fir init*/
		texture_path = exe_path + "/../models/fir/Fir.3DS";
		model_fir = new Model(texture_path, false);

		/* Palm init*/
		texture_path = exe_path + "/../models/palm/palm1.obj";
		model_palm = new Model(texture_path, false);
	}
	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		//projection = aProjection;
		//view = aView;

		mesh_shader->use();
		mesh_shader->setMat4("projection", aProjection);
		mesh_shader->setMat4("view", aView);

	}
	void Visit(Tree *tree)
	{
		// no general tree..
	};
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
		model = glm::translate(model, glm::vec3(0.0f,0.0f,0.3f));

		model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
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
};