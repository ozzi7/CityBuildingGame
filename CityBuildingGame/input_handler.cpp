#include "stdafx.h"
#include "input_handler.h"
#include "game_event_handler.h"
#include "sound_event_handler.h"

void InputHandler::Keypress(int button, int action) const
{
	if (windowFocused)
	{
		if (button == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(Window, true);

		// Keyboard scrolling, maybe to be removed
		if (button == GLFW_KEY_W || button == GLFW_KEY_UP)
			Camera->Scroll(CameraMovement::Up, 0.05f);
		if (button == GLFW_KEY_S || button == GLFW_KEY_DOWN)
			Camera->Scroll(CameraMovement::Down, 0.05f);
		if (button == GLFW_KEY_A || button == GLFW_KEY_LEFT)
			Camera->Scroll(CameraMovement::Left, 0.05f);
		if (button == GLFW_KEY_D || button == GLFW_KEY_RIGHT)
			Camera->Scroll(CameraMovement::Right, 0.05f);
	}
}

void InputHandler::Mouseclick(int button, int action) const
{
	if (windowFocused)
	{
		if (!action == GLFW_PRESS)
		{
			return;
		}

		soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound)); // TODO:

		// Test Code
		glm::vec3 cursor_position = Camera->CursorPositionOnGrid();
		if (cursor_position.x >= 0 && cursor_position.y >= 0 && Grid->gridHeight > (int)cursor_position.y && Grid->
			gridWidth > (int)cursor_position.x)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				gameEventHandler->AddEvent(
					new CreateBuildingEvent(BuildingType::LumberjackHutID, cursor_position.x, cursor_position.y));
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				gameEventHandler->AddEvent(new CreateBuildingEvent(BuildingType::DwellingID, cursor_position.x, cursor_position.y));
			}
		}
	}
}

void InputHandler::Mousewheel(float yOffset) const
{
	if (windowFocused)
		Camera->Zoom(yOffset);
}

void InputHandler::WindowFocus(int focused)
{
	windowFocused = focused;
	if (windowFocused)
	{
		int width;
		int height;
		int left;
		int top;
		RECT windowEdges;

		glfwGetWindowPos(Window, &left, &top);
		glfwGetWindowSize(Window, &width, &height);
		int right = left + width;
		int bottom = top + height;

		windowEdges.left = left;
		windowEdges.top = top;
		windowEdges.right = right;
		windowEdges.bottom = bottom;

		ClipCursor(&windowEdges);
	}
}

void InputHandler::MouseScroll() const
{
	if (windowFocused)
	{
		double xpos;
		double ypos;
		int width;
		int height;

		glfwGetCursorPos(Window, &xpos, &ypos);
		glfwGetWindowSize(Window, &width, &height);

		if (xpos == 0)
			Camera->Scroll(CameraMovement::Left, 0.01f);
		if (ypos == 0)
			Camera->Scroll(CameraMovement::Up, 0.01f);
		if (width - (int)xpos <= 1)
			Camera->Scroll(CameraMovement::Right, 0.01f);
		if (height - (int)ypos <= 1)
			Camera->Scroll(CameraMovement::Down, 0.01f);
	}
}
