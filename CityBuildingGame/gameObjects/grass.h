#pragma once
#include <rendered_object.h>
#include "visitor.h"

class Grass : public RenderedObject
{
public:
	Grass(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: RenderedObject(aPosition, aScale, aRotation) { };

	void Accept(Visitor& v) override
	{
		v.Visit(this);
	};
};
