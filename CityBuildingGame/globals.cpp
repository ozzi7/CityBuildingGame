#include "stdafx.h"
#include "globals.h"


/************************************************
 *  For now, call this to overwrite config
 ***********************************************/

unsigned int DEFAULT_SCREEN_WIDTH;
unsigned int DEFAULT_SCREEN_HEIGHT;

//unsigned int MAP_WIDTH;
//unsigned int MAP_HEIGHT;

void LoadFromConfigFile() {
	int ival;
	double dval;

	INI::File ft;

	// Loading from file
	if (!ft.Load(Path + "/../settings.ini"))
	{
		std::cout << Path + "/../settings.ini could not be opened, exiting..." << std::endl; // TODO we could try to create one
	}
	else
	{
		std::cout << "Loading settings from " + Path + "/../settings.ini..." << std::endl;

		DEFAULT_SCREEN_WIDTH = ft.GetSection("Graphics")->GetValue("DEFAULT_SCREEN_WIDTH", 1920).AsInt();
		DEFAULT_SCREEN_HEIGHT = ft.GetSection("Graphics")->GetValue("DEFAULT_SCREEN_HEIGHT", 1080).AsInt();

		//DEFAULT_SCREEN_WIDTH = ft.GetSection("Map")->GetValue("MAP_WIDTH", 200).AsInt();
		//DEFAULT_SCREEN_HEIGHT = ft.GetSection("Map")->GetValue("MAP_WIDTH", 200).AsInt();

		// Example overwrite 
		//ft.SetValue("Graphics:DEFAULT_SCREEN_WIDTH", 1920);
		//ft.Save(Path + "/../settings.ini");
	}
}
