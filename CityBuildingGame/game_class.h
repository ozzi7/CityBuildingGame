# pragma once
#include <stdlib.h>
#include <stdio.h>

#include "terrain.h"

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "common.h"

class GameClass
{
public:
	GameClass(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight);
	~GameClass();

	// Called by the application class to update the game logic
	void UpdateWorld();

	// Called by the application class when the window need to be redrawn.
	void Draw();

private:
	// The terrain
	Terrain terrain;

	float screenWidth, screenHeight;

	GLuint mvp_handle;
};