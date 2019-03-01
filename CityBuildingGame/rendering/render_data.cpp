#include "stdafx.h"
#include "render_data.h"

RenderBuffer::RenderBuffer() {};
void RenderBuffer::SaveData(GameObject* obj) {
	switch (obj->type) {
		case tree: break;
			firModels.push_back(obj->model);
			break;
		case grass: 
			grassModels.push_back(obj->model); 
			break;
		default: break;
	}
}
void RenderBuffer::ClearData()
{
	firModels.clear();
	grassModels.clear();
}