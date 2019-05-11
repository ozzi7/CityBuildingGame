#include "stdafx.h"
#include "render_data.h"

RenderBuffer::RenderBuffer() {};
void RenderBuffer::Visit(GameObject* obj) {}

void RenderBuffer::Visit(Pine* obj)
{
	pineModels.push_back(obj->model);
}

void RenderBuffer::Visit(Spruce* obj)
{
	spruceModels.push_back(obj->model);
}

void RenderBuffer::Visit(Oak* obj)
{
	oakModels.push_back(obj->model);
}

void RenderBuffer::Visit(Juniper* obj)
{
	juniperModels.push_back(obj->model);
}

void RenderBuffer::Visit(Lumberjack* obj)
{
	if (obj->visible)
	{
		lumberjackModels.push_back(obj->model);
		lumberjackAnimationSeconds.push_back(obj->animationSecond);
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
	terrain = obj;
}

void RenderBuffer::Visit(Tree* obj) {}

void RenderBuffer::ClearData()
{
	pineModels.clear();
	spruceModels.clear();
	juniperModels.clear();
	oakModels.clear();
	lumberjackModels.clear();
	lumberjackHutModels.clear();
	dwellingModels_growth1.clear();
	buildingOutlineModels.clear();
	settlerModels.clear();

	settlerAnimationSeconds.clear();
	lumberjackAnimationSeconds.clear();
}
