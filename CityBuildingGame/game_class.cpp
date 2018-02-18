#include "game_class.h"

GameClass::GameClass(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight)
{
	screenWidth = aScreenWidth;
	screenHeight = aScreenHeight;

	terrain.Initialize(aMapWidth, aMapHeight);
}

GameClass::~GameClass()
{
}

void GameClass::Draw()
{	
	terrain.Draw();
}

void GameClass::UpdateWorld()
{
}