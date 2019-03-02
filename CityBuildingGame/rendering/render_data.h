#pragma once
#include <vector>
#include "buffer.h"
#include <game_object.h>
#include <lumberjack.h>
#include <fir.h>

class RenderBuffer : public Buffer, public Visitor {
public:
	RenderBuffer();
	void Visit(GameObject*);
	void Visit(Fir*);
	void Visit(Lumberjack*);
	void Visit(Terrain* obj);
	void Visit(Tree* obj);

	void ClearData();

	std::vector<glm::mat4> firModels;
	std::vector<glm::mat4> grassModels;
	Terrain* terrain; // TODO: 
};
