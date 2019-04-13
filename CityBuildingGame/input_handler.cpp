#include "stdafx.h"
#include "input_handler.h"
#include "game_event_handler.h"
#include "sound_event_handler.h"

void InputHandler::Keypress(int button, int action)
{
	if (windowFocused)
	{
		if (button == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(Window, true);

		// Keyboard scrolling, maybe to be removed
		if (button == GLFW_KEY_W || button == GLFW_KEY_UP)
			Camera->Scroll(UP, 0.05f);
		if (button == GLFW_KEY_S || button == GLFW_KEY_DOWN)
			Camera->Scroll(DOWN, 0.05f);
		if (button == GLFW_KEY_A || button == GLFW_KEY_LEFT)
			Camera->Scroll(LEFT, 0.05f);
		if (button == GLFW_KEY_D || button == GLFW_KEY_RIGHT)
			Camera->Scroll(RIGHT, 0.05f);
	}
}
void InputHandler::Mouseclick(int button, int action) 
{
	if (windowFocused)
	{
		if (!action == GLFW_PRESS) { return; }

		// Test Code
		glm::vec3 cursor_position = Camera->CursorPositionOnGrid();
		if (cursor_position.x >= 0 && cursor_position.y >= 0 && Grid->gridHeight > (int)cursor_position.y && Grid->gridWidth > (int)cursor_position.x)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				/*Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->objects.push_back(
					new Pine(glm::vec3(cursor_position),
						glm::vec3(0.01f, 0.01f, 0.01f),
						glm::vec3(1.5707963f, 0.0f, 0.0f)));*/
				unitEventHandler->AddEvent(new CreateBuildingEvent(LumberjackHutID, cursor_position.x, cursor_position.y));
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				unitEventHandler->AddEvent(new CreateBuildingEvent(DwellingID, cursor_position.x, cursor_position.y));
			}
		}
	}
}

void InputHandler::Mousewheel(float yOffset)
{
	if (windowFocused)
		Camera->Zoom(yOffset);
}

void InputHandler::WindowFocus(int focused) 
{
	windowFocused = focused;
	if (windowFocused) {

		int width;
		int height;
		int left;
		int top;
		int right;
		int bottom;
		RECT windowEdges;

		glfwGetWindowPos(Window, &left, &top);
		glfwGetWindowSize(Window, &width, &height);
		right = left + width;
		bottom = top + height;

		windowEdges.left = left;
		windowEdges.top = top;
		windowEdges.right = right;
		windowEdges.bottom = bottom;

		ClipCursor(&windowEdges);
	}
}

void InputHandler::MouseScroll()
{
	if (windowFocused) {
		double xpos;
		double ypos;
		int width;
		int height;

		glfwGetCursorPos(Window, &xpos, &ypos);
		glfwGetWindowSize(Window, &width, &height);

		if (xpos == 0)
			Camera->Scroll(LEFT, 0.01f);
		if (ypos == 0)
			Camera->Scroll(UP, 0.01f);
		if (width - (int)xpos <= 1)
			Camera->Scroll(RIGHT, 0.01f);
		if (height - (int)ypos <= 1)
			Camera->Scroll(DOWN, 0.01f);
	}
}