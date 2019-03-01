#pragma once
#include <vector>
#include "buffer.h"
#include "game_object.h"


class RenderBuffer : public Buffer {
public:
	RenderBuffer();
	void SaveRenderData(GameObject* obj);

	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
};
