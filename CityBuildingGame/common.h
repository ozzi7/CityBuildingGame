#pragma once
#include "texture_loader.h"
#include "game_class.h"

class Common {
public:
	// settings
	static const unsigned int SCR_WIDTH = 1920;
	static const unsigned int SCR_HEIGHT = 1080;
	static float SCREEN_RATIO;
	// render time, admin machine 16 sek from pressing debug to show render at 300x300 (for reference)
	static const unsigned int MAP_WIDTH = 300;
	static const unsigned int MAP_HEIGHT = 400;


	static GameClass gameClass;
	static TextureLoader texture_loader;
	static std::string exe_path;
};