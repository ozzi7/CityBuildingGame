#include "stdafx.h"
#include "render_data.h"

RenderBuffer::RenderBuffer() {};
void RenderBuffer::Visit(GameObject* obj) {

}
void RenderBuffer::Visit(Fir* obj) {
	firModels.push_back(obj->model);
}
void RenderBuffer::Visit(Lumberjack* obj) {
}
void RenderBuffer::Visit(Terrain* obj) {

	terrain = obj;
}
void RenderBuffer::Visit(Tree* obj) {
}
void RenderBuffer::ClearData()
{
	firModels.clear();
	grassModels.clear();
}