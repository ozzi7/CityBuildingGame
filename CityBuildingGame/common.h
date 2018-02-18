#pragma once
#include <string>

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"

extern GLFWwindow* window;

/* This is a handle to the shader program */
extern Shader ourShader;

extern std::string exe_path;