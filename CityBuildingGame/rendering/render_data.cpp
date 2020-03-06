#include "stdafx.h"
#include "render_data.h"

RenderBuffer::RenderBuffer() {};
void RenderBuffer::Visit(GameObject* obj) {}

void RenderBuffer::Visit(Pine* obj)
{
	pineModels.push_back(obj->model);
}

void RenderBuffer::Visit(EuroBeech* obj)
{
	euroBeechModels.push_back(obj->model);
}
void RenderBuffer::Visit(EuroBeech2* obj)
{
	euroBeech2Models.push_back(obj->model);
}
void RenderBuffer::Visit(Oak* obj)
{
	oakModels.push_back(obj->model);
}

void RenderBuffer::Visit(Toona* obj)
{
	toonaModels.push_back(obj->model);
}

void RenderBuffer::Visit(Lumberjack* obj)
{
	if (obj->visible)
	{
		if (obj->state == State::returningHome) {
			lumberjackWoodModels.push_back(obj->model);
			lumberjackWoodAnimationSeconds.push_back(obj->animationSecond);
		}
		else if (obj->state == State::walkingToTarget) {
			lumberjackModels.push_back(obj->model);
			lumberjackAnimationSeconds.push_back(obj->animationSecond);
		}
		else {
			lumberjackChoppingModels.push_back(obj->model);
			lumberjackChoppingAnimationSeconds.push_back(obj->animationSecond);
		}
	}
}

void RenderBuffer::Visit(Settler* obj)
{
	if (obj->visible)
	{
		settlerModels.push_back(obj->model);
		settlerAnimationSeconds.push_back(obj->animationSecond);
	}
}

void RenderBuffer::Visit(LumberjackHut* obj)
{
	switch (obj->evolutionStage)
	{
	case 0:
		buildingOutlineModels.insert(buildingOutlineModels.end(), obj->buildingOutlines.begin(),
		                             obj->buildingOutlines.end());
		break;
	case 1:
		lumberjackHutModels.push_back(obj->model);
		break;
	}
}

void RenderBuffer::Visit(Dwelling* obj)
{
	switch (obj->evolutionStage)
	{
	case 0:
		buildingOutlineModels.insert(buildingOutlineModels.end(), obj->buildingOutlines.begin(),
		                             obj->buildingOutlines.end());
		break;
	case 1:
		dwellingModels_growth1.push_back(obj->model);
		break;
	}
}

void RenderBuffer::Visit(Building* obj) {}

void RenderBuffer::Visit(Terrain* obj)
{
	terrainVector.push_back(obj);
}

void RenderBuffer::Visit(Tree* obj) {}

void RenderBuffer::ClearData()
{
	pineModels.clear();
	euroBeechModels.clear();
	euroBeech2Models.clear();
	toonaModels.clear();
	oakModels.clear();
	lumberjackModels.clear();
	lumberjackWoodModels.clear();
	lumberjackChoppingModels.clear();
	lumberjackHutModels.clear();
	dwellingModels_growth1.clear();
	buildingOutlineModels.clear();
	settlerModels.clear();

	settlerAnimationSeconds.clear();
	lumberjackAnimationSeconds.clear();
	lumberjackWoodAnimationSeconds.clear();
	lumberjackChoppingAnimationSeconds.clear();

	terrainVector.clear();
}
