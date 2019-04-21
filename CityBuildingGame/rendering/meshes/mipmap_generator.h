#pragma once

#include "globals.h"

typedef std::pair<unsigned char, unsigned int> alphaPair;

class MipmapGenerator
{
public:
	MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight);

	void ScaleImage();

	unsigned char* resultUnrounded = nullptr;
	unsigned char* result = nullptr;
private:
	unsigned char bilinear(unsigned int positionWidth, unsigned int positionHeight, int channel);
	void roundAlpha();

	const unsigned int divisor = 2;
	unsigned int targetWidth, targetHeight;
	unsigned int resultCount;

	unsigned char* data;
	std::vector<alphaPair> alphaValues;
	unsigned int width, height;
};

