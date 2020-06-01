#include "stdafx.h"
#pragma once
#include <game_object.h>
#include <game_event_handler.h>
#include <sound_event_handler.h>
#include <resources.h>

GameObject::GameObject(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
{
	position = aPosition;
	scale = aScale;
	rotation = aRotation;
	recalculateModelMatix();

	posX = (int)position.x,
	posY = (int)position.y;
}

GameObject::GameObject() {};

void GameObject::recalculateModelMatix()
{
	model = glm::mat4(1.0f);
	model = translate(model, position);
	model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // TODO:
	model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, scale);
}
