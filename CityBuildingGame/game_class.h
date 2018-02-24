# pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

#include "terrain.h"

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "common.h"
#include "model.h"
#include "camera.h"
#include "shader.h"

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

	float screenWidth, screenHeight;

	Shader ourShader;
	Shader shaderTree;

private:
	// The terrain
	Terrain terrain;

	GLuint mvp_handle;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void ProcessInput(GLFWwindow * window);
};