#pragma once

#include <string>

// user settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const unsigned int MAP_WIDTH = 50;
const unsigned int MAP_HEIGHT = 50;

// rendering
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

/* map generation */
const float PINE_DENSITY = 5.0f;		// ~ 0 - 10
const float JUNIPER_DENSITY = 5.0f;
const float OAK_DENSITY = 5.0f;
const float SPRUCE_DENSITY = 5.0f;
const float HILL_HEIGHT = 7.0f;	// ~ 0 - 10
// high persistence => high frequency details have a greater effect
const float PERSISTENCE = 0.6f; // ~ 0.4 - 0.7
const float PLATEAU_PERCENTAGE = 10.0f; // [0 - 100] 
const float VALLEY_PERCENTAGE = 20.0f; //[0 - 100]

const float SCROLL_SPEED = 5.0f;
const float VISIBLE_RANGE = 1.6f;	// ~1.6 is entire screen
const float ZOOM_DEFAULT = 10.0f;

extern std::string Path;

const float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;