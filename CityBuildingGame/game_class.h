#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "common.h"
#include "camera.h"
#include "terrain.h"
#include "renderer.h"

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


private:
	// The terrain
	Terrain terrain;
	Renderer renderer; 

	GLuint mvp_handle;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void ProcessInput(GLFWwindow * window);
	extern GLFWwindow* window;

	/* This is a handle to the shader program */
	extern Camera camera;
	extern Model tree;

	extern std::string exe_path;

	// settings
	const unsigned int SCR_WIDTH = 1920;
	const unsigned int SCR_HEIGHT = 1080;
	const float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	// render time, admin machine 16 sek from pressing debug to show render at 300x300 (for reference)

	const unsigned int MAP_WIDTH = 300;
	const unsigned int MAP_HEIGHT = 400;
};