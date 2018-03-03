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
#include "terrain.h"
#include "tree.h"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>

class Game
{
public:
	Game::Game(int, int, float, string, Camera &, GLFWwindow* window);
	~Game();

	// Entry point to the game
	void StartGame();

	// Called by the application class to update the game logic
	void GameLoop();

	// Called by the application class when the window need to be redrawn.
	void RenderLoop();
	
	int mapHeight;
	int mapWidth;
	string exe_path;
	Camera * camera;

	GLFWwindow *window;

	/* This is a handle to the shader program */
	Model tree;

	Shader *shaderTree;
	Shader *shaderTerrain;

	float screenRatio;

private:
	Terrain *terrain;
	Model treeModel;
	Model terrainModel;

	vector<Tree*> trees;

	GLuint mvp_handle;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void ProcessInput();
};