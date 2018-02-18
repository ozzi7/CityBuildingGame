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
const float SPEED = 30.0f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Right;
	// Camera options
	float MovementSpeed;
	//float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) 
		:MovementSpeed(SPEED), Zoom(ZOOM)
	{
		Position = position;
		Up = up;
		Right = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ) 
		:MovementSpeed(SPEED), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		Up = glm::vec3(upX, upY, upZ);
		Right = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Right, Up);
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
		if (Zoom >= 1.0f && Zoom <= 50.0f)
			Zoom -= yoffset;
	}
};
#endif

