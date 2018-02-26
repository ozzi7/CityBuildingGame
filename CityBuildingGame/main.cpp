// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

// Include GLEW, used to detect supported openGL extensions
#include <GL\glew.h>

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>

#include "game_class.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
// render time, admin machine 16 sek from pressing debug to show render at 300x300 (for reference)
const unsigned int MAP_WIDTH = 30;
const unsigned int MAP_HEIGHT = 40;

//Camera camera;

int main(int argc, char* argv[])
{
	float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	// store path for resources
	std::string exe_path = std::string(argv[0]);
	exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CityBuildingGame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	Camera camera = Camera(glm::vec3(20.0f, 0.0f, 50.0f), window);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	/* Specify callbacks */
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	if (glewInit() != 0)
	{
		exit(EXIT_FAILURE);
	}

	GameClass gameClass(MAP_WIDTH, MAP_HEIGHT, SCREEN_RATIO, exe_path, camera, *window);
	gameClass.StartGame();

	glfwTerminate();
	return 0;
}
