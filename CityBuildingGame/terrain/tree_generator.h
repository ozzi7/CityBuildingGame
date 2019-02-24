#pragma once
#include <vector>

#include "gameObjects/tree.h"
#include "gameObjects/fir.h"

class TreeGenerator
{
public:
	TreeGenerator();
	std::vector<Tree> TreeGenerator::GenerateTrees();
};

