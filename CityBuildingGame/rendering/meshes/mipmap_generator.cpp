#include "stdafx.h"
#include "mipmap_generator.h"


MipmapGenerator::MipmapGenerator()
{
	cv::Mat img = cv::imread("test.png");
}


MipmapGenerator::~MipmapGenerator()
{
}
