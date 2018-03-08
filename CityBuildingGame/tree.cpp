#include "tree.h"

using namespace std;

Tree::Tree(glm::vec3 aPosition, glm::vec3 aScale, float aRotation) {
	position = aPosition;
	scale = aScale;
	rotation = aRotation;
}
void Tree::Accept(Visitor &v)
{
	v.Visit(this);
}