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
	void CreateBuildingPreviews() const;

	GLFWwindow* Window;
	Camera* Camera;
	Grid* Grid;


private:
	bool windowFocused = true;
	enum class BuildingSelection { Notset, Road, Dwelling, LumberjackHut };
	BuildingSelection buildingSelection = BuildingSelection::Notset; // -1 = not in building mode
	
	// for multi click events like building roads
	bool isLeftMouseClickDown = false;
	glm::vec3 firstKeyPressPosition;
};
