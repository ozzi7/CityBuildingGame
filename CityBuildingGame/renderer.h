#pragma once
#include "terrain.h"
#include "tree.h"

class Renderer
{
public:
	void Render(const Terrain & obj);
	void Render(const Tree & eobj);
};