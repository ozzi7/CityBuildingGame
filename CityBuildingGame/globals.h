#pragma once

#include <string>

// user settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const unsigned int MAP_WIDTH = 600;
const unsigned int MAP_HEIGHT = 400;

const float FIR_DENSITY = 5.0f;		// ~ 0 - 10
const float HILL_HEIGHT = 5.0f;	// ~ 0 - 10

const float SCROLL_SPEED = 5.0f;
const float VISIBLE_RANGE = 1.6f;	// ~1.6 is entire screen
const float ZOOM_DEFAULT = 10.0f;

extern std::string Path;


const float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;