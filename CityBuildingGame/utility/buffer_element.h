#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class BufferElement
{
public:
	BufferElement();
	~BufferElement();

	template <class T>
	void AddElement(T entry);
};