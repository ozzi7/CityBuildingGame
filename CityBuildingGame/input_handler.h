#pragma once

#include "camera.h"
#include "grid.h"

class InputHandler
{
public:
	void Mouseclick(int button, int action);
	void Keypress(int button, int action);
	void Mousewheel(float yOffset);
	void WindowFocus(int focused);
	void MouseScroll();
	
	GLFWwindow* Window;
	Camera* Camera;
	Grid* Grid;

private:
	bool windowFocused;
};
