#include "input_handler.h"

void InputHandler::Keypress(int button, int action)
{
	if (button == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(Window, true);
	if (button == GLFW_KEY_W || button == GLFW_KEY_UP)
		Camera->keyboard_scroll(UP);
	if (button == GLFW_KEY_S || button == GLFW_KEY_DOWN)
		Camera->keyboard_scroll(DOWN);
	if (button == GLFW_KEY_A || button == GLFW_KEY_LEFT)
		Camera->keyboard_scroll(LEFT);
	if (button == GLFW_KEY_D || button == GLFW_KEY_RIGHT)
		Camera->keyboard_scroll(RIGHT);
}

void InputHandler::Mouseclick(int button, int action) 
{

	if (!action == GLFW_PRESS) { return; }

	glm::vec2 cursor_position = Camera->CursorPositionGrid();

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		try {
			if (cursor_position.x >= 0 && cursor_position.y >= 0 && Grid->gridUnits.at((int)cursor_position.y).at((int)cursor_position.x)) {
				//Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->objects.push_back(
				//	new Fir(glm::vec3(cursor_position.x, cursor_position.y, Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->averageHeight),
				//		glm::vec3(1.0f, 1.0f, 1.0f),
				//		1.0f));

				/* TODO: temp */
				for (int i = 0; i < Grid->gridUnits.size(); i++) {
					for (int j = 0; j < Grid->gridUnits[i].size(); j++) {
						for (list<BoneAnimated*>::iterator it = Grid->gridUnits[i][j]->movingObjects.begin();
							it != Grid->gridUnits[i][j]->movingObjects.end(); ++it) {
							// to create path to walk
							static_cast<Lumberjack*>(*it)->wayPoints.push_back(glm::vec3(cursor_position.x, cursor_position.y, Grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->averageHeight));
						}
					}
				}
			}
		}
		catch (const std::out_of_range & e) {
			std::cout << "Cannot insert outside of grid" << std::endl;
		}

	}
}

void InputHandler::Mousewheel(float yOffset)
{
	Camera->mouse_zoom(yOffset);
}

void InputHandler::WindowFocus(int focused) 
{
	if (focused) {
		Camera->lock_cursor_to_window();
		Camera->WindowFocused = true;
	}
	else {
		Camera->WindowFocused = false;
	}
}

void InputHandler::MouseScroll()
{
	Camera->mouse_scroll();
}