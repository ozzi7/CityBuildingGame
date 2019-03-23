#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "visitor.h"
#include <event_visitor.h>
#include <event_handler.h>

class EventHandler;

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 model = glm::mat4(1.0f);

	int posX = -1; // this must be set, otherwise crash pls
	int posY = -1;


	virtual void Accept(class Visitor &v) = 0;

protected:
	void recalculateModelMatix();
};