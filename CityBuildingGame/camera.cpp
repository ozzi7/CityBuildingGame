#include "stdafx.h"
#include "camera.h"

Camera::Camera() {};

Camera::Camera(glm::vec3 position, GLFWwindow* aWindow)
{
	Position = position;
	ZoomLevel = ZOOM_DEFAULT;
	window = aWindow;
	up = glm::vec3(-1.0f, 1.0f, 0.0f);
	right = glm::vec3(1.0f, 1.0f, 0.0f);
	lookat = glm::vec3(-50.0f, 50.0f, -50.0f);

}

// Returns the view matrix calculated using LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + lookat, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::ortho(-SCREEN_RATIO * ZoomLevel, SCREEN_RATIO * ZoomLevel, -1.0f * ZoomLevel, 1.0f * ZoomLevel, 0.0f, 200.0f);
}

// Top left position on Grid that is visible by camera
glm::vec2 Camera::GridTopLeftVisible()
{
	float x = Position.x + lookat.x - ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y - ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Top right position on Grid that is visible by camera
glm::vec2 Camera::GridTopRightVisible()
{
	float x = Position.x + lookat.x + ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y + ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom left position on Grid that is visible by camera
glm::vec2 Camera::GridBottomLeftVisible()
{
	float x = Position.x + lookat.x - ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y - ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom right position on Grid that is visible by camera
glm::vec2 Camera::GridBottomRightVisible()
{
	float x = Position.x + lookat.x + ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y + ZoomLevel * 0.5f * SCREEN_RATIO * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Current cursor position on Grid
glm::vec3 Camera::CursorPositionOnGrid()
{
	float x, y, z;
	double window_x, window_y;

	glfwGetCursorPos(window, &window_x, &window_y);
	x = (float)window_x;
	y = SCR_HEIGHT - (float)window_y;
	glReadPixels((int)x, (int)y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	
	glm::vec3 window = glm::vec3(x, y, z);
	glm::mat4 model = GetViewMatrix();
	glm::mat4 projection = GetProjectionMatrix();
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT);
	
	glm::vec3 result = glm::unProject(window, model, projection, viewport);

	return result;
}

void Camera::Scroll(Camera_Movement direction, float yOffset)
{
	if (direction == UP)
		Position += up * yOffset * ZoomLevel * ROOT3;
	if (direction == DOWN)
		Position -= up * yOffset * ZoomLevel * ROOT3;
	if (direction == LEFT)
		Position -= right * yOffset * ZoomLevel;
	if (direction == RIGHT)
		Position += right * yOffset * ZoomLevel;
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

