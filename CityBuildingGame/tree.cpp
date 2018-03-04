#include "tree.h"

using namespace std;


Tree::Tree(glm::vec3 position) {
	Position = position;
}
void Tree::ChangeSize()
{

}
void Tree::Draw()
{
	model->DrawTree(this);
}