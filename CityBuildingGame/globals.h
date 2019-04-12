#pragma once

#include <string>
#include <event_handler.h>

// user settings
const unsigned int DEFAULT_SCREEN_WIDTH = 1920;
const unsigned int DEFAULT_SCREEN_HEIGHT = 1080;

const unsigned int MAP_WIDTH = 200;
const unsigned int MAP_HEIGHT = 200;

// rendering
const unsigned int SHADOW_WIDTH = 1920;
const unsigned int SHADOW_HEIGHT = 1080;

/* vegetation generation */
// the higher the tree persistence the higher the frequency of trees => easier to find all types of resources nearby
const float PERSISTENCE_TREES = 0.8f;

// defines on which height the species grows the most in percentage of the terrain height
const float PINE_GAUSSIAN_MEAN_PERCENTAGE = 82.0f;		// [0 - 100]
const float JUNIPER_GAUSSIAN_MEAN_PERCENTAGE = 57.0f;		// [0 - 100]
const float OAK_GAUSSIAN_MEAN_PERCENTAGE = 37.0f;		// [0 - 100]
const float SPRUCE_GAUSSIAN_MEAN_PERCENTAGE = 72.0f;		// [0 - 100]

// high variance => tree can grow at a larger spectrum of height around its mean
const float PINE_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]
const float JUNIPER_GAUSSIAN_VARIANCE_PERCENTAGE = 1.0f;		// [0 - 100]
const float OAK_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]
const float SPRUCE_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]

const float TREE_SCALE_FACTOR = 0.011f; // to resize all trees
const float SMALL_TREE_CUTOFF_PERCENTAGE = 30.0f; // [0,100), removes midget trees
const float PINE_DENSITY = 0.3f;		//0.4 [0 - 100]
const float JUNIPER_DENSITY = 0.05f;		//0.1 [0 - 100]
const float OAK_DENSITY = 0.3f;		// 0.4[0 - 100]
const float SPRUCE_DENSITY= 0.3f;		//0.4 [0 - 100]

/* terrain generation */
const float HILL_HEIGHT = 15.0f;	// ~ 0 - 15

// high persistence => high frequency details have a greater effect
const float PERSISTENCE = 0.5f; // ~ 0.4 - 0.7

const float PLATEAU_PERCENTAGE = 10.0f; // [0 - 100] flattens x percent tallest terrain
const float VALLEY_PERCENTAGE = 30.0f; //[0 - 100] flattens x percent of lowest terrain

// if 0 => the tree noise map defines tree distribution, 1=> the terrain height defines tree distribution
const float TERRAIN_WEIGHT_FACTOR = 0.7f; // [0,1] 

const float SCROLL_SPEED = 5.0f;
const float VISIBLE_RANGE = 1.6f;	// ~1.6 is entire screen
const float ZOOM_DEFAULT = 10.0f;

extern std::string Path;

extern unsigned int ScreenWidth;
extern unsigned int ScreenHeight;
extern float ScreenRatio;
// TODO: gameloop fps

extern EventHandler* unitEventHandler; // ?? save in every object?! same with grid?