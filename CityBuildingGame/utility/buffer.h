#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


class Buffer
{
public:
	Buffer();
	~Buffer();

	template <class T>
	void ClearData();
	template <class T>
	void SaveData(T entry);
};