#pragma once
#include <vector>
#include "buffer.h"
#include <game_object.h>
#include <lumberjack.h>
#include <spruce.h>
#include <juniper.h>
#include <oak.h>
#include <pine.h>

class RenderBuffer : public Buffer, public Visitor {
public:
	RenderBuffer();
	void Visit(GameObject*);
	void Visit(Spruce*);
	void Visit(Oak*);
	void Visit(Pine*);
	void Visit(Juniper*);
	void Visit(Lumberjack*);
	void Visit(Terrain* obj);
	void Visit(Tree* obj);
	void Visit(Building* obj);
	void Visit(LumberjackHut* obj);

	void ClearData();

	std::vector<glm::mat4> pineModels;
	std::vector<glm::mat4> spruceModels;
	std::vector<glm::mat4> juniperModels;
	std::vector<glm::mat4> oakModels;
	std::vector<glm::mat4> lumberjackModels;

	Terrain* terrain; // TODO: 
};
