#include "stdafx.h"
#include "render_data.h"

RenderBuffer::RenderBuffer() {};
void RenderBuffer::Visit(GameObject* obj) {

}
void RenderBuffer::Visit(Pine* obj) {
	pineModels.push_back(obj->model);
}
void RenderBuffer::Visit(Spruce* obj) {
	spruceModels.push_back(obj->model);
}
void RenderBuffer::Visit(Oak* obj) {
	oakModels.push_back(obj->model);
}
void RenderBuffer::Visit(Juniper* obj) {
	juniperModels.push_back(obj->model);
}
void RenderBuffer::Visit(Lumberjack* obj) {
	lumberjackModels.push_back(obj->model);
}
void RenderBuffer::Visit(LumberjackHut* obj) {
	lumberjackHutModels.push_back(obj->model);
}
void RenderBuffer::Visit(Dwelling* obj) {
	dwellingModels.push_back(obj->model);
}
void RenderBuffer::Visit(Building* obj) {
	
}
void RenderBuffer::Visit(Terrain* obj) {

	terrain = obj;
}
void RenderBuffer::Visit(Tree* obj) {
}
void RenderBuffer::ClearData()
{
	pineModels.clear();
	spruceModels.clear();
	juniperModels.clear();
	oakModels.clear();
	lumberjackModels.clear();
	lumberjackHutModels.clear();
	dwellingModels.clear();
}