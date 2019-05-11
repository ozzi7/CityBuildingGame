#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <Windows.h>
#include <iostream>

#include "globals.h"
#include "grid.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct LightSource
{
	glm::vec3 PositionOffset;
	glm::vec3 Direction;
	glm::vec3 Color;
};

const float ZOOM_MAX = 50.0f;
const float ZOOM_MIN = 0.2f;
const float ROOT3 = (float)sqrt(3); // ROOT3 if viewing angle is 45°

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, GLFWwindow* window);

	glm::vec3 Position;
	float ZoomLevel = ZOOM_DEFAULT;
	Grid* Grid;
	LightSource DirectionalLight;

	glm::mat4 GetViewMatrix();			// Returns the view matrix calculated using LookAt Matrix
	glm::mat4 GetLightViewMatrix();
	glm::mat4 GetProjectionMatrix();	// Returns the projection orthographic projection matrix
	glm::mat4 GetLightProjectionMatrix();
	glm::vec2 GridTopLeftVisible();
	glm::vec2 GridTopRightVisible();
	glm::vec2 GridBottomLeftVisible();
	glm::vec2 GridBottomRightVisible();
	glm::vec3 CursorPositionOnGrid();

	void Scroll(Camera_Movement, float);
	void Zoom(float);
	void UpdateLightDirection();		// Call this after LightSource Position update

	float projectionIncrease = 2.0f;

private:
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 lookat;
	GLFWwindow* window;
};
