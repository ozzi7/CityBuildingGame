# pragma once
#ifndef CAMERA_H
#define CAMERA_H

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// Default camera values
const float SPEED = 40.0f;
const float SENSITIVTY = 0.1f;
const float ZOOM_DEFAULT = 40.0f;
const float ZOOM_MAX = 60.0f;
const float ZOOM_MIN = 5.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 Lookat;
	// Camera options
	float MovementSpeed;
	float Zoom;

	// Constructor with position vector
	Camera(glm::vec3 position) 
	{
		Position = position;
		Zoom = ZOOM_DEFAULT;
		MovementSpeed = SPEED;
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Right = glm::vec3(1.0f, 0.0f, 0.0f);
		Lookat = glm::vec3(0.0f, 20.0f, -30.0f);
	}

	// Returns the view matrix calculated using LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Lookat, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= ZOOM_MIN && Zoom <= ZOOM_MAX)
			Zoom -= yoffset;
		if (Zoom <= ZOOM_MIN)
			Zoom = ZOOM_MIN;
		if (Zoom >= ZOOM_MAX)
			Zoom = ZOOM_MAX;
	}
};
#endif

