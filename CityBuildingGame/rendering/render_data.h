#pragma once
#include <vector>
#include "buffer_element.h"

class RenderBufferElement : public BufferElement {
public:
	RenderBufferElement();
	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
};
