#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

enum RenderObjects { Lumberjack, Fir, Grass };

class Renderdata
{
public:
	glm::mat4 model;
	RenderObjects type = Fir;
};
