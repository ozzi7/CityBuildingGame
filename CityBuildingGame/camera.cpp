#include "stdafx.h"
#pragma once
#include "camera.h"

Camera::Camera(const glm::vec3 position, GLFWwindow* window)
{
	Position = position;
	Window = window;

	Up = glm::vec3(-1.0f, 1.0f, 0.0f);
	Right = glm::vec3(1.0f, 1.0f, 0.0f);
	LookAt = glm::vec3(-50.0f, 50.0f, -50.0f);
}

// Returns the view matrix calculated using LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const
{
	return lookAt(Position, Position + LookAt, Up);
}

glm::mat4 Camera::GetLightViewMatrix() const
{
	//return glm::lookAt(DirectionalLight.Position, DirectionalLight.Position + lookAt, glm::vec3(0.0, 0.0, 1.0));
	return lookAt(Position + LookAt + DirectionalLight.PositionOffset, Position + LookAt, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::ortho(-ScreenRatio * ZoomLevel, ScreenRatio * ZoomLevel, -1.0f * ZoomLevel, ZoomLevel, 0.0f, 200.0f);
}

glm::mat4 Camera::GetLightProjectionMatrix() const
{
	return glm::ortho(-ZoomLevel * ProjectionIncrease, ZoomLevel * ProjectionIncrease,
	                  -1.0f * ZoomLevel * ProjectionIncrease, ZoomLevel * ProjectionIncrease, -100.0f, 100.0f);
}

// Top left position on Grid that is visible by camera
glm::vec2 Camera::GridTopLeftVisible() const
{
	const float x = Position.x + LookAt.x - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;
	const float y = Position.y + LookAt.y - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Top right position on Grid that is visible by camera
glm::vec2 Camera::GridTopRightVisible() const
{
	const float x = Position.x + LookAt.x + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;
	const float y = Position.y + LookAt.y + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom left position on Grid that is visible by camera
glm::vec2 Camera::GridBottomLeftVisible() const
{
	const float x = Position.x + LookAt.x - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;
	const float y = Position.y + LookAt.y - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom right position on Grid that is visible by camera
glm::vec2 Camera::GridBottomRightVisible() const
{
	const float x = Position.x + LookAt.x + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;
	const float y = Position.y + LookAt.y + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 *
		VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Current cursor position on Grid
glm::vec3 Camera::CursorPositionOnGrid() const
{
	float z;
	double windowX, windowY;

	glfwGetCursorPos(Window, &windowX, &windowY);
	const float x = (float)windowX;
	const float y = (float)ScreenHeight - (float)windowY;
	glReadPixels((int)x, (int)y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	const glm::vec3 window = glm::vec3(x, y, z);
	const glm::mat4 model = GetViewMatrix();
	const glm::mat4 projection = GetProjectionMatrix();
	const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)ScreenWidth, (float)ScreenHeight);

	glm::vec3 result = unProject(window, model, projection, viewport);

	return result;
}

void Camera::Scroll(Camera_Movement direction, float yOffset)
{
	if (direction == UP)
		Position += Up * yOffset * ZoomLevel * ROOT3;
	if (direction == DOWN)
		Position -= Up * yOffset * ZoomLevel * ROOT3;
	if (direction == LEFT)
		Position -= Right * yOffset * ZoomLevel;
	if (direction == RIGHT)
		Position += Right * yOffset * ZoomLevel;
}

void Camera::Zoom(float yOffset)
{
	if (ZoomLevel >= ZOOM_MIN && ZoomLevel <= ZOOM_MAX)
		ZoomLevel -= yOffset * ZoomLevel / 10.0f;
	if (ZoomLevel <= ZOOM_MIN)
		ZoomLevel = ZOOM_MIN;
	if (ZoomLevel >= ZOOM_MAX)
		ZoomLevel = ZOOM_MAX;
}

void Camera::UpdateLightDirection()
{
	/*float distance =
		sqrt(
		pow((lookAt.x - Position.x + DirectionalLight.PositionOffset.x), 2) +
		pow((lookAt.y - Position.y + DirectionalLight.PositionOffset.y), 2) +
		pow((lookAt.z - Position.z + DirectionalLight.PositionOffset.z), 2));*/

	DirectionalLight.Direction = -DirectionalLight.PositionOffset;
}
