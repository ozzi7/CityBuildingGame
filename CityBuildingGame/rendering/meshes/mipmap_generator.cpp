#include "stdafx.h"
#include "mipmap_generator.h"


MipmapGenerator::MipmapGenerator(unsigned char* aData, unsigned int aWidth, unsigned int aHeight)
{
	data = aData;
	width = aWidth;
	height = aHeight;
}

unsigned char* MipmapGenerator::ScaledImage()
{
	const unsigned int divisor = 2; //(unsigned int)pow(2, level);
	const unsigned int targetWidth = width / divisor;
	const unsigned int targetHeight = height / divisor;
	const unsigned int resultCount = targetWidth * targetHeight * 4;
	unsigned char* result = new unsigned char[resultCount];

	unsigned int i = 0;
	unsigned int currentWidth = 1;
	unsigned int currentHeight = 1;
	
	while (i < resultCount) {
		for (int channel = 0; channel <= 3; channel++)
		{
			unsigned char color = bilinear(currentWidth, currentHeight, channel, divisor);

			if (channel == 3)
				if (color <= 127)
					color = 0;
				else
					color = 255;

			result[i] = color;
			//std::cout << "Position: " << i << " Value: " << +result[i] << std::endl;
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
	return result;
}

unsigned char MipmapGenerator::bilinear(unsigned int positionWidth, unsigned int positionHeight, int channel, unsigned int divisor)
{
	float accurateWidth = (positionWidth - 0.5f) * divisor + 0.5f;
	float accurateHeight = (positionHeight - 0.5f) * divisor + 0.5f;

	unsigned char upperLeft = data[((int)accurateHeight - 1) * width * 4 + ((int)accurateWidth - 1) * 4 + channel];
	unsigned char upperRight = data[((int)accurateHeight - 1) * width * 4 + (int)accurateWidth * 4  + channel];
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

	//if (color > 0) {
	//	std::cout << "Upper Left: " << ((int)accurateHeight - 1) * width * 4 + ((int)accurateWidth - 1) * 4 + channel << " Value: " << (float)upperLeft << std::endl;
	//	std::cout << "Upper Right: " << ((int)accurateHeight - 1) * width * 4 + (int)accurateWidth * 4 + channel << " Value: " << (float)upperRight << std::endl;
	//	std::cout << "Lower Left: " << (int)accurateHeight * width * 4 + ((int)accurateWidth - 1) * 4 + channel << " Value: " << (float)lowerLeft << std::endl;
	//	std::cout << "Lower Right: " << (int)accurateHeight * width * 4 + (int)accurateWidth * 4 + channel << " Value: " << (float)lowerRight << std::endl;
	//}
	return (unsigned char)round(color);
}
