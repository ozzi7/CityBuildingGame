#include "game_class.h"

GameClass::GameClass(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight)
{
	screenWidth = aScreenHeight;
	screenHeight = aMapWidth;

	terrain.Initialize(aMapWidth, aMapHeight);
}

GameClass::~GameClass()
{
}

void GameClass::Draw()
{	
	//InitCamera();
	terrain.Draw();
}

void GameClass::UpdateWorld()
{
}