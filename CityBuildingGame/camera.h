# pragma once
#ifndef CAMERA_H
#define CAMERA_H

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <vector>
#include <Windows.h>
#include <iostream>

#include "grid.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

const float SCROLL_SPEED = 0.1f;
const float ZOOM_DEFAULT = 5.0f;
const float ZOOM_MAX = 100.0f;
const float ZOOM_MIN = 0.2f;
const float VISIBLE_RANGE = 1.4f; // ~1.5 is entire screen
const float ROOT3 = (float)sqrt(3); // ROOT3 if viewing angle is 45°


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	glm::vec3 Position;
	float Zoom;
	bool WindowFocused = true;
	Grid * grid;
private:
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 Lookat;
	GLFWwindow *Window;
	RECT WindowEdges;
	float ScreenRatio;

public:
	Camera() {}

	Camera(glm::vec3 position, GLFWwindow *window)
	{
		Position = position;
		Zoom = ZOOM_DEFAULT;
		Window = window;
		Up = glm::vec3(-1.0f, 1.0f, 0.0f);
		Right = glm::vec3(1.0f, 1.0f, 0.0f);
		Lookat = glm::vec3(-50.0f, 50.0f, -50.0f);

		lock_cursor_to_window();
	}

	// Returns the view matrix calculated using LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Lookat, Up);
	}

	glm::vec2 GetTopLeftVisible()
	{
		float x = Position.x + Lookat.x - Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE - Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;
		float y = Position.y + Lookat.y - Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE + Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;

		return glm::vec2(x, y);
	}

	glm::vec2 GetTopRightVisible()
	{
		float x = Position.x + Lookat.x + Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE - Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;
		float y = Position.y + Lookat.y + Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE + Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;

		return glm::vec2(x, y);
	}

	glm::vec2 GetBottomLeftVisible()
	{
		float x = Position.x + Lookat.x - Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE + Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;
		float y = Position.y + Lookat.y - Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE - Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;

		return glm::vec2(x, y);
	}

	glm::vec2 GetBottomRightVisible()
	{
		float x = Position.x + Lookat.x + Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE + Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;
		float y = Position.y + Lookat.y + Zoom * 0.5f * ScreenRatio * VISIBLE_RANGE - Zoom * 0.5f * ROOT3 * VISIBLE_RANGE;

		return glm::vec2(x, y);
	}

	glm::vec2 CursorPositionGrid()
	{
		glm::vec2 gridPosition;
		float z = 0.0f;
		float y;
		float x;

		gridPosition = cursorPosition(z);

		y = gridPosition.y;
		x = gridPosition.x;

		// Outside of grid cases
		if (y < 0) y = 0;
		if (x < 0) x = 0;
		if (y > (float)grid->gridUnits.size()) y = (float)grid->gridUnits.size();
		if (x > (float)grid->gridUnits[0].size()) x = (float)grid->gridUnits[0].size();
		
		z = grid->gridUnits[(int)y][(int)x]->averageHeight;

		std::cout << "(x|y) (" << gridPosition.x << "|" << gridPosition.y << ")" << std::endl;

		x += z;
		y -= z;

		// Check if actual grid unit is different because of height
		if ((int)x != (int)(x - z) || (int)y != (int)(y + z)) {

			// Outside of grid cases
			if (y < 0) y = 0;
			if (x < 0) x = 0;
			if (y >(float)grid->gridUnits.size()) y = (float)grid->gridUnits.size();
			if (x >(float)grid->gridUnits[0].size()) x = (float)grid->gridUnits[0].size();

			z = grid->gridUnits[(int)y][(int)x]->averageHeight;

			std::cout << "Adjusted grid unit" << std::endl;
		}

		gridPosition.x += z;
		gridPosition.y -= z;

		std::cout << "(x|y) adjusted (" << gridPosition.x << "|" << gridPosition.y << ") by height z " << z << std::endl;

		return gridPosition;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void keyboard_scroll(Camera_Movement direction)
	{
		if (direction == UP)
			Position += Up * SCROLL_SPEED * ROOT3;
		if (direction == DOWN)
			Position -= Up * SCROLL_SPEED * ROOT3;
		if (direction == LEFT)
			Position -= Right * SCROLL_SPEED;
		if (direction == RIGHT)
			Position += Right * SCROLL_SPEED;
	}

	// Processes scrolling when mouse reaches edge of window
	void mouse_scroll()
	{
		double xpos;
		double ypos;
		int width;
		int height;

		if (!WindowFocused) {
			return;
		}

		glfwGetCursorPos(Window, &xpos, &ypos); 
		glfwGetWindowSize(Window, &width, &height);

		if (xpos == 0)
			Position -= Right * SCROLL_SPEED;
		if (ypos == 0)
			Position += Up * SCROLL_SPEED * ROOT3;
		if (width-(int)xpos <= 1)
			Position += Right * SCROLL_SPEED;
		if (height-(int)ypos <= 1)
			Position -= Up * SCROLL_SPEED * ROOT3;
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void mouse_zoom(float yoffset)
	{
		if (Zoom >= ZOOM_MIN && Zoom <= ZOOM_MAX)
			Zoom -= yoffset;
		if (Zoom <= ZOOM_MIN)
			Zoom = ZOOM_MIN;
		if (Zoom >= ZOOM_MAX)
			Zoom = ZOOM_MAX;
	}

	// Lock curser to current window size
	void lock_cursor_to_window()
	{
		int width;
		int height;
		int left;
		int top;
		int right;
		int bottom;

		if (!glfwGetWindowAttrib(Window, GLFW_FOCUSED)) {
			return;
		}

		glfwGetWindowPos(Window, &left, &top);
		glfwGetWindowSize(Window, &width, &height);
		right = left + width;
		bottom = top + height;

		WindowEdges.left = left;
		WindowEdges.top = top;
		WindowEdges.right = right;
		WindowEdges.bottom = bottom;

		ScreenRatio = (float)width / (float)height;

		ClipCursor(&WindowEdges);
	}
	
private:
	glm::vec2 cursorPosition(float z)
	{
		int window_width, window_height;
		double window_x, window_y;
		GLfloat x = 0.0f, y = 0.0f;

		glfwGetCursorPos(Window, &window_x, &window_y);
		glfwGetWindowSize(Window, &window_width, &window_height);


		x += ((float)window_x - window_width / 2); // Cursor offset from middle of screen in X direction
		y += ((float)window_x - window_width / 2);

		x += ((float)window_y - window_height / 2) * ROOT3; // Cursor offset from middle of screen in Y direction
		y -= ((float)window_y - window_height / 2) * ROOT3; // Screen coordinate system starts at top

		x *= (Zoom / window_width) * 2.575f; // Adjust for zoom
		y *= (Zoom / window_width) * 2.575f; // 2.575 = magic number

		x += z;	// Adjust for z height
		y -= z;

		x += Position.x + Lookat.x; // Camera offset
		y += Position.y + Lookat.y;

		return glm::vec2(x, y);
	}
};
#endif

