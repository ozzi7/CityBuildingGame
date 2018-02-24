#pragma once
#include <string>
#include <algorithm>

// Include GLEW, used to detect supported openGL extensions
#include <GL\glew.h>

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include "model.h"

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

class Common {
public:
	static unsigned int loadTexture(std::string filename);
};