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
	Camera() = default;
	Camera(glm::vec3 position, GLFWwindow* window);

	glm::vec3 Position;
	float ZoomLevel = ZOOM_DEFAULT;
	Grid* Grid{};
	LightSource DirectionalLight;

	glm::mat4 GetViewMatrix() const;			// Returns the view matrix calculated using LookAt Matrix
	glm::mat4 GetLightViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;	// Returns the projection orthographic projection matrix
	glm::mat4 GetLightProjectionMatrix() const;
	glm::vec2 GridTopLeftVisible() const;
	glm::vec2 GridTopRightVisible() const;
	glm::vec2 GridBottomLeftVisible() const;
	glm::vec2 GridBottomRightVisible() const;
	glm::vec3 CursorPositionOnGrid() const;

	void Scroll(Camera_Movement, float);
	void Zoom(float);
	void UpdateLightDirection();		// Call this after LightSource Position update

	float ProjectionIncrease = 2.0f;

private:
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 LookAt;
	GLFWwindow* Window{};
};
