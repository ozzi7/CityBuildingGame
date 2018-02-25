#pragma once
#include "texture_loader.h"
#include "game_class.h"

class Common {
public:
	GameClass gameClass;
	TextureLoader texture_loader;
	std::string exe_path;
};