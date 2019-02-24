#pragma once
#include <vector>

#include "tree.h"
#include "fir.h"

class TreeGenerator
{
public:
	TreeGenerator();
	std::vector<Tree> TreeGenerator::GenerateTrees();
};

