// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>

// Include GLEW, used to detect supported openGL extensions
#include <GL/glew.h>

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "game.h"
#include "input_handler.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int MAP_WIDTH = 200;
const unsigned int MAP_HEIGHT = 100;
const float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;

Game game;
GLFWwindow* window;
InputHandler inputHandler;

void init_glfw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_focus_callback(GLFWwindow *window, int focused);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main(int argc, char* argv[])
{
	// store path for resources
	std::string exe_path = std::string(argv[0]);
	exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));
	std::replace(exe_path.begin(), exe_path.end(), '\\', '/');

	init_glfw();

	game = Game(MAP_WIDTH, MAP_HEIGHT, SCREEN_RATIO, exe_path, window, &inputHandler);
	game.StartGame();

	glfwTerminate();
	return 0;
}

void init_glfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 4); /* MSAA */

	// glfw window creation
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CityBuildingGame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	/* Specify callbacks */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	if (glewInit() != 0)
	{
		std::cout << "Failed to init GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	inputHandler.Mousewheel((float)yoffset);
}

void window_focus_callback(GLFWwindow *window, int focused)
{
	inputHandler.WindowFocus(focused);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	inputHandler.Mouseclick(button, action);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputHandler.Keypress(key, action);
}