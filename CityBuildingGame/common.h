#pragma once
#include <string>
#include <algorithm>

// Include GLEW, used to detect supported openGL extensions
#include <GL\glew.h>

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"

extern GLFWwindow* window;

/* This is a handle to the shader program */
extern Shader ourShader;

extern std::string exe_path;

class Common {
public:
	static unsigned int loadTexture(std::string filename);
};