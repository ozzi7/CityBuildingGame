#pragma once
#include "texture_loader.h"
#include "game_class.h"

extern class Common {
public:
	// settings
	const unsigned int SCR_WIDTH = 1920;
	const unsigned int SCR_HEIGHT = 1080;
	float SCREEN_RATIO;
	// render time, admin machine 16 sek from pressing debug to show render at 300x300 (for reference)
	const unsigned int MAP_WIDTH = 300;
	unsigned int MAP_HEIGHT = 400;

	Common();
	~Common();
	GameClass gameClass;
	TextureLoader texture_loader;
	std::string exe_path;
	Camera camera;
};