#pragma once

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "tree.h"
#include "white_tree.h"
#include "visitor.h"

#include "shader.h"
#include "model.h"

class Renderer : public Visitor
{
public:
	Model *model_tree;
	Shader *shader_tree;

	glm::mat4 projection;
	glm::mat4 view;

	void SetMatrices(glm::mat4 aProjection, glm::mat4 aView)
	{
		projection = aProjection;
		view = aView;
	}
	void Visit(Tree *tree)
	{
		shader_tree->use();
		shader_tree->setMat4("projection", projection);
		shader_tree->setMat4("view", view);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, tree->Position);
		model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));

		shader_tree->setMat4("model", model2);

		model_tree->Draw(*shader_tree);
	};
	void Visit(WhiteTree *white_tree)
	{
		shader_tree->use();
		shader_tree->setMat4("projection", projection);
		shader_tree->setMat4("view", view);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, white_tree->Position);
		model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));

		shader_tree->setMat4("model", model2);

		model_tree->Draw(*shader_tree);
	};
};