#pragma once

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "tree.h"
#include "white_tree.h"
#include "visitor.h"
#include "fir.h"
#include "shader.h"
#include "model.h"

class Renderer : public Visitor
{
public:
	Model *model_white_tree;
	Model *model_fir;
	Shader *shader_white_tree;
	Shader *shader_fir;

	glm::mat4 projection;
	glm::mat4 view;

	Renderer(std::string exe_path)
	{
		/* white tree init*/
		shader_white_tree = new Shader("vertex_shader.vert", "fragment_shader.frag");

		std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
		std::string texture_path = exe_path + "/tree2_3ds/Tree2.3ds";
		model_white_tree = new Model(texture_path, false);

		/* fir init*/
		shader_fir = new Shader("vertex_shader.vert", "fragment_shader.frag");

		texture_path = exe_path + "/fir/Fir.obj";
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
	void Visit(WhiteTree *white_tree)
	{
		shader_white_tree->use();
		shader_white_tree->setMat4("projection", projection);
		shader_white_tree->setMat4("view", view);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, white_tree->Position);
		model2 = glm::scale(model2, glm::vec3(0.07f, 0.07f, 0.07f));

		shader_white_tree->setMat4("model", model2);

		model_white_tree->Draw(*shader_white_tree);
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