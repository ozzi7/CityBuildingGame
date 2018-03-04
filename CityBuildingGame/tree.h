#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include "model.h"

class Model;


class Tree
{
	public:
		glm::vec3 Position;

		Tree(glm::vec3);
		void ChangeSize();
		void Draw();
		static Model* model;
		static Shader* shader;
};