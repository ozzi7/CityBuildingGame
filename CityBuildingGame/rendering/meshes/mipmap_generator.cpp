#include "stdafx.h"
#include "mipmap_generator.h"

MipmapGenerator::MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight)
{
	data = aData;
	width = aWidth;
	height = aHeight;

	targetWidth = width / divisor;
	targetHeight = height / divisor;
	resultCount = targetWidth * targetHeight * 4;
	result = new unsigned char[resultCount];
	resultUnrounded = new unsigned char[resultCount];
	alphaValues.resize(targetWidth * targetHeight);
}

void MipmapGenerator::ScaleImage()
{
	unsigned int i = 0;
	unsigned int alphaCounter = 0;
	unsigned int currentWidth = 1;
	unsigned int currentHeight = 1;

	while (i < resultCount) {
		for (int channel = 0; channel <= 3; channel++)
		{
			unsigned char color = bilinear(currentWidth, currentHeight, channel);

			if (channel == 3)
			{
				alphaValues[alphaCounter] = alphaPair(color, i);
				alphaCounter++;
			}

			resultUnrounded[i] = color;
			result[i] = color;
			i++;
		}
		if (currentWidth < targetWidth)
			currentWidth++;
		else
		{
			currentWidth = 1;
			currentHeight++;
		}
	}
	roundAlpha();
}

unsigned char MipmapGenerator::bilinear(unsigned int positionWidth, unsigned int positionHeight, int channel)
{
	float accurateWidth = (positionWidth - 0.5f) * divisor + 0.5f;
	float accurateHeight = (positionHeight - 0.5f) * divisor + 0.5f;

	unsigned char upperLeft = data[((int)accurateHeight - 1) * width * 4 + ((int)accurateWidth - 1) * 4 + channel];
	unsigned char upperRight = data[((int)accurateHeight - 1) * width * 4 + (int)accurateWidth * 4 + channel];
	unsigned char lowerLeft = data[(int)accurateHeight * width * 4 + ((int)accurateWidth - 1) * 4 + channel];
	unsigned char lowerRight = data[(int)accurateHeight * width * 4 + (int)accurateWidth * 4 + channel];

	float lowerWidthWeight = accurateWidth - (int)accurateWidth;
	float upperWidthWeight = 1.0f - lowerWidthWeight;
	float lowerHeightWeight = accurateHeight - (int)accurateHeight;
	float upperHeightWeight = 1.0f - lowerHeightWeight;

	float upperLeftWeight = lowerWidthWeight * upperHeightWeight;
	float upperRightWeight = upperWidthWeight * upperHeightWeight;
	float lowerLeftWeight = lowerWidthWeight * lowerHeightWeight;
	float lowerRightWeight = upperWidthWeight * lowerHeightWeight;

	float color = upperLeft * upperLeftWeight + upperRight * upperRightWeight + lowerLeft * lowerLeftWeight + lowerRight * lowerRightWeight;

	return (unsigned char)round(color);
}

void MipmapGenerator::roundAlpha()
{
	std::sort(alphaValues.begin(), alphaValues.end());

	unsigned int removedAlpha = 0;
	unsigned int addedAlpha = 0;

	unsigned int frontCounter = 0;
	unsigned int backCounter = alphaValues.size() - 1;

	unsigned int position;

	while (frontCounter < backCounter)
	{
		if (addedAlpha > removedAlpha)
		{
			// Read from front with lowest Alpha
			position = alphaValues[frontCounter].second;

			result[position] = 0;
			removedAlpha += alphaValues[frontCounter].first;

			frontCounter++;
		}
		else
		{
			// Read from back with highest Alpha
			position = alphaValues[backCounter].second;

			result[position] = 255;
			addedAlpha += 255 - alphaValues[backCounter].first;

			backCounter--;
		}
	}
}