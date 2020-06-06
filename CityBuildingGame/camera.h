#pragma once

#define NOMINMAX

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <algorithm>

#include "globals.h"
#include "grid.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement
{
	Up,
	Down,
	Left,
	Right
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
	glm::vec3 AmbientLight;

	LightSource GetDirectionalLight() const;

	void SetDirectionalLightPositionOffset(glm::vec3 coordinateOffset);
	void SetDirectionalLightColor(glm::vec3 color);

	// Returns the view matrix, calculated using LookAt Matrix
	glm::mat4 GetViewMatrix() const;
	
	// Returns the view matrix of directional light, calculated using LookAt Matrix
	glm::mat4 GetLightViewMatrix() const;

	// Returns the orthographic projection matrix
	glm::mat4 GetProjectionMatrix() const;

	// Returns the projection matrix of directional light
	glm::mat4 GetLightProjectionMatrix() const;

	// Top left position on Grid that is visible by camera
	glm::vec2 GridTopLeftVisible() const;

	// Top right position on Grid that is visible by camera
	glm::vec2 GridTopRightVisible() const;

	// Bottom left position on Grid that is visible by camera
	glm::vec2 GridBottomLeftVisible() const;

	// Bottom right position on Grid that is visible by camera
	glm::vec2 GridBottomRightVisible() const;

	// Current cursor position on Grid
	glm::vec3 CursorPositionOnGrid() const;

	// Calculates edges of visible grid, call this before getting grid visible coordinates
	void CalculateVisibleGrid();

	// Calculates the viewport for the directional light, call this after changing the DirectionalLight.PositionOffset
	void CalculateLightProjectionMatrix();

	// Move position on map
	void Scroll(CameraMovement, float);
	void Zoom(float);

private:
	int MaxX;
	int MaxY;
	glm::vec3 VecUp;
	glm::vec3 VecRight;
	glm::vec3 LookAt;
	LightSource DirectionalLight;
	glm::mat4 LightProjectionMatrix;
	GLFWwindow* Window{};
	std::pair<int, int> TopLeftVisible, TopRightVisible, BottomLeftVisible, BottomRightVisible;

	// Gets exact game coordinate from a screen coordinate, can only be called from openGL thread
	glm::vec3 PixelTo3DCoordinate(const glm::vec2 pixelCoordinates) const;

	// Gets exact game coordinate from a screen coordinate and z buffer depth
	glm::vec3 PixelTo3DCoordinate(glm::vec2 pixelCoordinates, float z) const;

	// Determines if a 3D coordinate is visible on the screen
	bool CoordinateVisible(glm::vec3 coordinate) const;

	// Determines if a 3D coordinate is visible by render matrices
	static bool CoordinateVisible(glm::vec3 coordinate, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
};
