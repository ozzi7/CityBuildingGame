#pragma once
#include "globals.h"

class Shadow
{
public:
	Shadow();

	void InitShadowMap();
	void BindShadowMap() const;
	void UnbindShadowMap();
	unsigned int DepthMap;

private:
	unsigned int depthMapFBO;
};
