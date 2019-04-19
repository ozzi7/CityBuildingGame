#include "stdafx.h"
#include "mipmap_generator.h"


MipmapGenerator::MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight)
{
	data = aData;
	width = aWidth;
	height = aHeight;
}

unsigned char* MipmapGenerator::ScaledImage(unsigned int level)
{
	const unsigned int divisor = pow(2, level);
	const unsigned int resultCount = width * height * 4 / divisor;
	const unsigned int targetWidth = width / divisor;
	const unsigned int targetHeight = height / divisor;
	unsigned char* result = new unsigned char[resultCount];

	unsigned int i = 0;
	unsigned int currentWidth = 1;
	unsigned int currentHeight = 1;
	
	while (i < resultCount) {
		for (int type = 0; type <= 3; type++)
		{
			std::cout << +(*result) << std::endl;
			char color = bilinear(currentWidth, currentHeight, divisor);

			if (type == 3)
				if (color < 128)
					color = 0;
				else
					color = 255;

			result[i] = color;

			i++;
		}
		if (currentWidth < targetWidth)
			currentWidth++;
		else
		{
			currentWidth = 0;
			currentHeight++;
		}
	}
	return result;
}

char MipmapGenerator::bilinear(unsigned int positionWidth, unsigned int positionHeight, unsigned int divisor)
{
	float accurateWidth = (positionWidth - 0.5f) * divisor + 0.5f;
	float accurateHeight = (positionHeight - 0.5f) * divisor + 0.5f;

	char lowerWidth = data[(int)accurateWidth];
	char upperWidth = data[(int)accurateWidth + 1];
	float lowerWidthWeight = accurateWidth - (int)accurateWidth;
	float upperWidthWeight = 1.0f - lowerWidthWeight;

	char lowerHeight = data[(int)accurateHeight];
	char upperHeight = data[(int)accurateHeight + 1];
	float lowerHeightWeight = accurateHeight - (int)accurateHeight;
	float upperHeightWeight = 1.0f - lowerHeightWeight;

	float color = (lowerWidth * lowerWidthWeight + upperWidth * upperWidthWeight + lowerHeight * lowerHeightWeight + upperHeight * upperHeightWeight) * 0.5f;

	return (char)color;
}
