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
#include "tree.h"

class GameClass
{
public:
	GameClass();
	~GameClass();
	
	void Init(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight);

	// Entry point to the game
	void StartGame();

	// Called by the application class to update the game logic
	void GameLoop();

	// Called by the application class when the window need to be redrawn.
	void RenderLoop();

	float screenWidth, screenHeight;

	GLFWwindow* window;

	/* This is a handle to the shader program */
	Camera camera;
	Model tree;

	Shader shaderTree = Shader("vertex_shader.vert", "fragment_shader.frag");


private:
	// The terrain
	Terrain & terrain;

	Model & treeModel;

	vector<Tree &> trees;

	GLuint mvp_handle;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void ProcessInput(GLFWwindow * window);
};