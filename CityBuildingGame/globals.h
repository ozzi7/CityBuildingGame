#pragma once

#include <string>
#include <game_event_handler.h>
#include <sound_event_handler.h>
#include <logging_event_handler.h>
#include <resources.h>
#include <fstream>
#include "iniparser.hpp"


/************************************************
 *  Graphics settings
 ***********************************************/

extern unsigned int DEFAULT_SCREEN_WIDTH;
extern unsigned int DEFAULT_SCREEN_HEIGHT;

const bool SHADOW_ENABLED = true; // TODO: not implemented yet
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

extern float BRIGHTNESS;  // [0 - 100]

const bool CUSTOM_MIPMAPS = true;


/************************************************
 *  Sound settings
 ***********************************************/
extern bool SOUND_ENABLED;
extern bool MUSIC_ENABLED;


/************************************************
 *  Map settings
 ***********************************************/
//extern unsigned int MAP_WIDTH;
//extern unsigned int MAP_HEIGHT;
const unsigned int MAP_WIDTH = 100;
const unsigned int MAP_HEIGHT = 100;

/* vegetation generation */
// the higher the tree persistence the higher the frequency of trees => easier to find all types of resources nearby
const float PERSISTENCE_TREES = 0.8f;
const float PERSISTENCE_GRASS = 0.8f;

// defines on which height the species grows the most in percentage of the terrain height
const float PINE_GAUSSIAN_MEAN_PERCENTAGE = 82.0f;		// [0 - 100]
const float TOONA_GAUSSIAN_MEAN_PERCENTAGE = 57.0f;	// [0 - 100]
const float OAK_GAUSSIAN_MEAN_PERCENTAGE = 37.0f;		// [0 - 100]
const float EUROBEECH_GAUSSIAN_MEAN_PERCENTAGE = 72.0f;	// [0 - 100]
const float GRASS_GAUSSIAN_MEAN_PERCENTAGE = 10.0f;	// [0 - 100]

// high variance => tree can grow at a larger spectrum of height around its mean
const float PINE_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]
const float TOONA_GAUSSIAN_VARIANCE_PERCENTAGE = 1.0f;	// [0 - 100]
const float OAK_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]
const float EUROBEECH_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]
const float GRASS_GAUSSIAN_VARIANCE_PERCENTAGE = 5.0f;		// [0 - 100]

const float TREE_SCALE_FACTOR = 0.015f; // to resize all trees
const float GRASS_SCALE_FACTOR = 0.4f;

const float SMALL_TREE_CUTOFF_PERCENTAGE = 30.0f; // [0,100), removes midget trees
const float SMALL_GRASS_CUTOFF_PERCENTAGE = 60.0f; // [0,100)

const float PINE_DENSITY = 0.15f;		//0.3 [0 - 1]
const float TOONA_DENSITY = 0.15f;		//0.3 [0 - 1]
const float OAK_DENSITY = 0.05f;		//0.15 [0 - 1]
const float EUROBEECH_DENSITY = 0.15f;		//0.3 [0 - 1]
const float GRASS_DENSITY = 0.4f;

/* terrain generation */
const float HILL_HEIGHT = 15.0f;	// ~ 0 - 15

// high persistence => high frequency details have a greater effect
const float PERSISTENCE = 0.5f; // ~ 0.4 - 0.7

const float PLATEAU_PERCENTAGE = 10.0f; // [0 - 100] flattens x percent tallest terrain
const float VALLEY_PERCENTAGE = 50.0f; //[0 - 100] flattens x percent of lowest terrain

// if 0 => the tree noise map defines tree distribution, 1=> the terrain height defines tree distribution
const float TERRAIN_WEIGHT_FACTOR = 0.7f; // [0,1]


/************************************************
 *  Game settings
 ***********************************************/
const float SCROLL_SPEED = 10.0f;
const float ZOOM_DEFAULT = 10.0f;
extern std::string LOGGING_LEVEL_FILE;
extern std::string LOGGING_LEVEL_CONSOLE;

extern std::string Path;

extern unsigned int ScreenWidth;
extern unsigned int ScreenHeight;
extern float ScreenRatio;
// TODO: gameloop fps

extern GameEventHandler* gameEventHandler;
extern SoundEventHandler* soundEventHandler;
extern LoggingEventHandler* loggingEventHandler;

extern Resources* resources;

void LoadFromConfigFile();