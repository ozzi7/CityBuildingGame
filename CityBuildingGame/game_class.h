# pragma once
#include <stdlib.h>
#include <stdio.h>
#include <thread>

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

	// Entry point to the game
	void StartGame();

	// Called by the application class to update the game logic
	void GameLoop();

	// Called by the application class when the window need to be redrawn.
	void RenderLoop();
	void RenderLoop(float interpolation);

private:
	// The terrain
	Terrain terrain;

	float screenWidth, screenHeight;

	GLuint mvp_handle;
};