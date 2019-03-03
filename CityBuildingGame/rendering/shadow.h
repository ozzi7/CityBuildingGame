#pragma once
#include "globals.h"

class Shadow
{
public:
	Shadow();

	void InitShadowMap();
	void BindShadowMap();

private:
	unsigned int depthMapFBO;
	unsigned int depthMap;
};

