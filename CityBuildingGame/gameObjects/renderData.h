#pragma once
#include <vector>
#include <triple_buffer.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class RenderData {
public:
	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
};