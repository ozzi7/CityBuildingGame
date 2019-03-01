#pragma once
#include <vector>
#include "buffer.h"

enum RenderObjType { gameObject, fir, grass, lumberjack, tree,  boneAnimated };

class RenderBuffer : public Buffer {
public:
	RenderBuffer();
	void SaveRenderData(GameObject*);

	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
};
