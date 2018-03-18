#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"
#include "camera.h"
#include "grid.h"
#include "tree.h"
#include "renderer.h"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>

class Game
{
public:
	Game(int, int, float, string, Camera &, GLFWwindow* window);
	Game();
	~Game();

	// Entry point to the game
	void StartGame();

	// Called by the application class to update the game logic
	void GameLoop();

	// Continuously redraws all visible objects
	void RenderLoop();
	// Continuously recalculates bones of all visible objects
	void BoneCalcLoop();

	void ProcessMouseclick(int button, int action, int mods);
	
	int mapHeight;
	int mapWidth;
	string exe_path;
	Camera * camera;

	GLFWwindow *window;

	/* This is a handle to the shader program */
	Model tree;

	float screenRatio;

private:
	Renderer *renderer;
	Grid *grid;
	Model whiteTreeModel;
	Model firTreeModel;
	Model terrainModel;

	GLuint mvp_handle;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void ProcessInput();
};