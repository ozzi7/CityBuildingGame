#pragma once
#include "buffer_element.h"
#include <vector>

class RenderBufferElement : public BufferElement {
public:
	RenderBufferElement();
	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
};
