#include "tree.h"

using namespace std;

Tree::Tree(glm::vec3 position) {
	Position = position;
}
void Tree::Accept(Visitor &v)
{
	v.Visit(this);
}