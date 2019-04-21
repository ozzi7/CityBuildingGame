#pragma once

#include <iostream>
#include "globals.h"

class MipmapGenerator
{
public:
	MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight);

	unsigned char* ScaledImage();

	unsigned char* resultUnrounded = nullptr;
	unsigned char* result = nullptr;
private:
	unsigned char bilinear(unsigned int positionWidth, unsigned int positionHeight, int channel, unsigned int divisor);
	void roundAlpha();

	unsigned char* data;
	unsigned int width, height;
};

