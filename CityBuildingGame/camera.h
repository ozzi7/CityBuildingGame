#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
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

const float SCROLL_SPEED = 0.02f;
const float ZOOM_DEFAULT = 10.0f;
const float ZOOM_MAX = 50.0f;
const float ZOOM_MIN = 0.2f;
const float VISIBLE_RANGE = 1.4f; // ~1.5 is entire screen
const float ROOT3 = (float)sqrt(3); // ROOT3 if viewing angle is 45°


class Camera
{
public:
	glm::vec3 Position;
	float ZoomLevel;
	Grid* Grid;
	float ScreenRatio;


	Camera();
	Camera(glm::vec3 position, GLFWwindow* window);

	// Returns the view matrix calculated using LookAt Matrix
	glm::mat4 GetViewMatrix();
	glm::vec2 GridTopLeftVisible();
	glm::vec2 GridTopRightVisible();
	glm::vec2 GridBottomLeftVisible();
	glm::vec2 GridBottomRightVisible();
	glm::vec2 CursorPositionOnGrid();
	void Scroll(Camera_Movement, float);
	void Zoom(float);

private:
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 lookat;
	GLFWwindow* window;


	glm::vec2 cursorPosition(float);
};

#endif

