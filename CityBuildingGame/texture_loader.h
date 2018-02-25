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
#include "common.h"

class TextureLoader {
public:
	static unsigned int loadTexture(std::string filename);
};