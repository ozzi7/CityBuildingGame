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

	glm::mat4 projection;
	glm::mat4 view;

	Renderer(std::string exe_path)
	{
		/* Chamaecyparis init*/
		shader_chamaecyparis = new Shader("mesh_shader.vert", "fragment_shader.frag");

		std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
		std::string texture_path = exe_path + "/Chamaecyparis/Tree Chamaecyparis N161216.3ds";
		model_chamaecyparis = new Model(texture_path, false);

		/* fir init*/
		shader_fir = new Shader("mesh_shader.vert", "mesh_shader.frag");

		texture_path = exe_path + "/fir/Fir.3DS";
		model_fir = new Model(texture_path, false);
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
		shader_chamaecyparis->use();
		shader_chamaecyparis->setMat4("projection", projection);
		shader_chamaecyparis->setMat4("view", view);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, chamaecyparis->Position);
		model2 = glm::rotate(model2, 90.0f, glm::vec3(1,0,0));
		model2 = glm::scale(model2, glm::vec3(0.02f, 0.02f, 0.02));

		shader_chamaecyparis->setMat4("model", model2);

		model_chamaecyparis->Draw(*shader_chamaecyparis);
	};
	void Visit(Fir *fir)
	{
		shader_fir->use();
		shader_fir->setMat4("projection", projection);
		shader_fir->setMat4("view", view);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, fir->Position);
		model2 = glm::scale(model2, glm::vec3(0.003f, 0.003f, 0.003f));

		shader_fir->setMat4("model", model2);

		model_fir->Draw(*shader_fir);
	};
};