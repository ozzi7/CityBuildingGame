#pragma once

#include <iostream>
#include <math.h>

class MipmapGenerator
{
public:
	MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight);

	unsigned char* ScaledImage(unsigned int level);

private:
	char bilinear(unsigned int positionWidth, unsigned int positionHeight, unsigned int divisor);

	unsigned char* data;
	unsigned int width, height;
};

