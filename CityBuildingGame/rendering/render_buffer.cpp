#include "stdafx.h"
#include "render_buffer.h"

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
void RenderBuffer::Visit(Grass* obj)
{
	grassModels.push_back(obj->model);
}

/*	goingToWork : lumberjack has axe and walks to tree
	goingToWorkplace: worker walks to lumberjackhut
	gettingResource : worker walks to resource to pick it up

	pending : waiting for game event to be processed

*/
// enum class State { idle, pending, immigrating, carryingWood, carryingStone, gettingWood, gettingStone, goingToWork, goingToWorkplace, working, walking };

void RenderBuffer::Visit(Lumberjack* obj)
{
	if (obj->visible)
	{
		if (obj->state == State::carryingWood) {
			lumberjackWoodModels.push_back(obj->model);
			lumberjackWoodAnimationSeconds.push_back(obj->animationSecond);
		}
		else if (obj->state == State::goingToWork) {
			lumberjackModels.push_back(obj->model);
			lumberjackAnimationSeconds.push_back(obj->animationSecond);
		}
		else if(obj->state == State::working){
			lumberjackChoppingModels.push_back(obj->model);
			lumberjackChoppingAnimationSeconds.push_back(obj->animationSecond);
		}
		// TODO debug proxy object
		//glm::mat4 model = glm::mat4(1.0f);
		//model = translate(model, glm::vec3(obj->proxyPosition.x, obj->proxyPosition.y, obj->position.z));
		//model = rotate(model, obj->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = rotate(model, obj->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // TODO:
		//model = rotate(model, obj->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, obj->scale);
		//lumberjackModels.push_back(model);
		//lumberjackAnimationSeconds.push_back(obj->animationSecond);
	}
}

void RenderBuffer::Visit(Worker* obj)
{
	if (obj->visible)
	{
		if (obj->state == State::carryingWood) {
			workerWoodModels.push_back(obj->model);
			workerWoodAnimationSeconds.push_back(obj->animationSecond);
		}
		else
		{ // not sure which states, but default worker model 
			workerModels.push_back(obj->model);
			workerAnimationSeconds.push_back(obj->animationSecond);
		}
		// TODO debug proxy object
		//glm::mat4 model = glm::mat4(1.0f);
		//model = translate(model, glm::vec3(obj->proxyPosition.x, obj->proxyPosition.y, obj->position.z));
		//model = rotate(model, obj->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = rotate(model, obj->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // TODO:
		//model = rotate(model, obj->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, obj->scale);
		//workerModels.push_back(model);
		//workerAnimationSeconds.push_back(obj->animationSecond);
	}
}

void RenderBuffer::Visit(LumberjackHut* obj)
{
	switch (obj->evolutionStage)
	{
	case 0:
		buildingOutlineModels.insert(buildingOutlineModels.end(), obj->buildingOutlines.begin(), 
		                             obj->buildingOutlines.end()); // TODO: or clear the source vector on evolve ?
		break;
	case 1:
		lumberjackHutModels.push_back(obj->model);
		break;
	}
	woodModels.insert(woodModels.end(), obj->woodModels.begin(), obj->woodModels.end());
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
	woodModels.insert(woodModels.end(), obj->woodModels.begin(), obj->woodModels.end());
}

void RenderBuffer::Visit(Building* obj) {}

void RenderBuffer::Visit(Terrain* obj)
{
	terrainVector.push_back(obj);
}

void RenderBuffer::Visit(Tree* obj) {}

void RenderBuffer::Visit(RenderedObject* obj) {}

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
	workerModels.clear();
	workerWoodModels.clear();

	grassModels.clear();

	woodModels.clear();

	workerAnimationSeconds.clear();
	lumberjackAnimationSeconds.clear();
	lumberjackWoodAnimationSeconds.clear();
	lumberjackChoppingAnimationSeconds.clear();
	workerWoodAnimationSeconds.clear();

	terrainVector.clear();
}
