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
			Camera->Scroll(CameraMovement::Up, 0.05f);
		if (button == GLFW_KEY_S || button == GLFW_KEY_DOWN)
			Camera->Scroll(CameraMovement::Down, 0.05f);
		if (button == GLFW_KEY_A || button == GLFW_KEY_LEFT)
			Camera->Scroll(CameraMovement::Left, 0.05f);
		if (button == GLFW_KEY_D || button == GLFW_KEY_RIGHT)
			Camera->Scroll(CameraMovement::Right, 0.05f);

		if (action == GLFW_PRESS) {

			BuildingSelection targetSelection = BuildingSelection::Notset;

			switch (button) { // assign keys here
				case GLFW_KEY_1:
					targetSelection = BuildingSelection::Road;
					break;
				case GLFW_KEY_2:
					targetSelection = BuildingSelection::Dwelling;
					break;
				case GLFW_KEY_3:
					targetSelection = BuildingSelection::LumberjackHut;
					break;
				default:
					return;
			}

			// Not in building mode yet
			if (buildingSelection == BuildingSelection::Notset)
			{
				Grid->buildingMode = true;
				buildingSelection = targetSelection;
				Grid->terrain->reloadTerrain = true;
				Grid->reloadGrid = true;
			}
			// In building mode of different building -> change to new 
			else if (buildingSelection != targetSelection)
			{
				buildingSelection = targetSelection;
			}
			// in building mode of current building -> toggle off
			else 
			{
				Grid->buildingMode = false;
				buildingSelection = BuildingSelection::Notset;
				Grid->terrain->reloadTerrain = true;
				Grid->reloadGrid = true;
			}
		}
	}
}

void InputHandler::Mouseclick(int button, int action)
{
	if (windowFocused)
	{
		if (action == GLFW_PRESS)
		{
			// Test Code
			const glm::vec3 cursorPosition = Camera->GetCursorPositionOnGrid();

			if (Grid->IsValidPosition(cursorPosition))
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT)
				{
					isLeftMouseClickDown = true;
					switch (buildingSelection)
					{
						case BuildingSelection::Road:
							firstKeyPressPosition = cursorPosition;
							break;
						default:
							break;
					}
				}
				else if (button == GLFW_MOUSE_BUTTON_RIGHT)
				{
					Grid->buildingMode = false;
					Grid->reloadGrid = true;
					buildingSelection = BuildingSelection::Notset;
					Grid->terrain->reloadTerrain = true;
					isLeftMouseClickDown = false;
				}
			}
		}
		else if (action == GLFW_RELEASE && isLeftMouseClickDown)
		{
			const glm::vec3 cursorPosition = Camera->GetCursorPositionOnGrid();
			isLeftMouseClickDown = false;
			
			if (Grid->IsValidPosition(cursorPosition))
			{
				switch (buildingSelection)
				{
					case BuildingSelection::Dwelling:
						gameEventHandler->AddEvent(new CreateBuildingEvent(BuildingType::DwellingID, cursorPosition.x, cursorPosition.y));
						break;
					case BuildingSelection::LumberjackHut:
						gameEventHandler->AddEvent(new CreateBuildingEvent(BuildingType::LumberjackHutID, cursorPosition.x, cursorPosition.y));
						break;
					case BuildingSelection::Road:
						gameEventHandler->AddEvent(new CreateBuildingEvent(BuildingType::PathID, firstKeyPressPosition.x,
							firstKeyPressPosition.y, cursorPosition.x, cursorPosition.y));
						break;
					default:
						break;
				}
			}
		}
	}
}
void InputHandler::CreateBuildingPreviews() const
{
	const glm::vec3 cursorPosition = Camera->GetCursorPositionOnGrid();

	if (Grid->IsValidPosition(cursorPosition))
	{
		if (buildingSelection == BuildingSelection::Notset)
		{
			Grid->previewObjects.clear();
			Grid->ClearRoadPreview();
		}
		if (buildingSelection == BuildingSelection::Dwelling)
			gameEventHandler->AddEvent(new CreateBuildingPreviewEvent(BuildingType::DwellingID, cursorPosition.x,
				cursorPosition.y));
		if (buildingSelection == BuildingSelection::LumberjackHut)
			gameEventHandler->AddEvent(new CreateBuildingPreviewEvent(BuildingType::LumberjackHutID, cursorPosition.x,
				cursorPosition.y));
		if (buildingSelection == BuildingSelection::Road)
			if (!isLeftMouseClickDown) 
				gameEventHandler->AddEvent(new CreateBuildingPreviewEvent(BuildingType::PathID, firstKeyPressPosition.x,
				                                                          firstKeyPressPosition.y, firstKeyPressPosition.x,
					firstKeyPressPosition.y));
			else 
				gameEventHandler->AddEvent(new CreateBuildingPreviewEvent(BuildingType::PathID, firstKeyPressPosition.x,
				                                                          firstKeyPressPosition.y, cursorPosition.x, cursorPosition.y));
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

		windowEdges.left = left;
		windowEdges.top = top;
		windowEdges.right = left + width;
		windowEdges.bottom = top + height;

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
