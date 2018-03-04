#pragma once
#include <vector>

#include "tree.h"
#include "fir.h"
#include "white_tree.h"

class TreeGenerator
{
public:
	TreeGenerator();
	std::vector<Tree> TreeGenerator::GenerateTrees();
};

