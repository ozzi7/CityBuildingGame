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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "grid.h"
#include "camera.h"
#include "renderer.h"
#include "input_handler.h"
#include "map_generator.h"

class Game
{
public:
	Game(GLFWwindow*, InputHandler*);
	Game();
	~Game();

	// Entry point to the game
	void StartGame();


private:
	Renderer* renderer;
	Shadow* shadow;
	Grid* grid;
	Camera* camera;
	Camera* lightSource;
	GLFWwindow* window;
	InputHandler* inputHandler;
	TripleBuffer<RenderBuffer>* renderBuffers;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	// Called by the application class to update the game logic
	void gameLoop();

	// Continuously redraws all visible objects
	void renderLoop();
};