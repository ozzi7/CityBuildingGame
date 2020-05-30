#pragma once

#include "camera.h"
#include "grid.h"

class InputHandler
{
public:
	void Mouseclick(int button, int action);
	void Keypress(int button, int action);
	void Mousewheel(float yOffset) const;
	void WindowFocus(int focused);
	void MouseScroll() const;

	GLFWwindow* Window;
	Camera* Camera;
	Grid* Grid;

private:
	bool windowFocused = true;
	int buildingSelection = -1; // -1 = not in building mode
};
