#pragma once
#include "tree.h"
#include "visitor.h"

class Fir: public Tree
{
public:
	Fir(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation) 
		: Tree(aPosition, aScale, aRotation) {

		//model = glm::translate(model, position);
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::scale(model, scale);
		//model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 1.5707963f, glm::vec3(1.0f, 0.0f, 0.0f));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};