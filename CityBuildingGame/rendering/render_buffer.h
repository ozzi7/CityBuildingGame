#pragma once
#include <vector>
#include "buffer.h"
#include <game_object.h>
#include <lumberjack.h>
#include <euro_beech.h>
#include "euro_beech_2.h"
#include <toona.h>
#include <oak.h>
#include <pine.h>
#include <lumberjack_hut.h>
#include <dwelling.h>
#include <worker.h>
#include <grass.h>

class RenderBuffer : public Buffer, public Visitor
{
public:
	RenderBuffer();
	void Visit(GameObject*);
	void Visit(EuroBeech*) override;
	void Visit(EuroBeech2*) override;
	void Visit(Oak*) override;
	void Visit(Pine*) override;
	void Visit(Toona*) override;
	void Visit(Grass* obj) override;
	void Visit(Lumberjack*) override;
	void Visit(Terrain* obj) override;
	void Visit(Tree* obj) override;
	void Visit(RenderedObject* obj) override;
	void Visit(Building* obj) override;
	void Visit(LumberjackHut* obj) override;
	void Visit(Dwelling* obj) override;
	void Visit(Worker* obj) override;
	void ClearData();

	// dont forget to clear the vectors too!
	std::vector<glm::mat4> pineModels;
	std::vector<glm::mat4> euroBeechModels;
	std::vector<glm::mat4> euroBeech2Models;
	std::vector<glm::mat4> toonaModels;
	std::vector<glm::mat4> oakModels;
	std::vector<glm::mat4> grassModels;
	std::vector<glm::mat4> lumberjackModels;
	std::vector<glm::mat4> lumberjackWoodModels;
	std::vector<glm::mat4> lumberjackHutModels;
	std::vector<glm::mat4> lumberjackChoppingModels;
	std::vector<glm::mat4> dwellingModels_growth1;
	std::vector<glm::mat4> buildingOutlineModels;
	std::vector<glm::mat4> workerModels;
	std::vector<glm::mat4> workerWoodModels;

	std::vector<glm::mat4> woodModels;

	std::vector<float> workerAnimationSeconds;
	std::vector<float> lumberjackAnimationSeconds;
	std::vector<float> lumberjackWoodAnimationSeconds;
	std::vector<float> lumberjackChoppingAnimationSeconds;
	std::vector<float> workerWoodAnimationSeconds;

	std::vector<Terrain*> terrainVector;
};
