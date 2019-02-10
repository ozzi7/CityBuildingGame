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

// Top left position on Grid that is visible by camera
glm::vec2 Camera::GridTopLeftVisible()
{
	float x = Position.x + lookat.x - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Top right position on Grid that is visible by camera
glm::vec2 Camera::GridTopRightVisible()
{
	float x = Position.x + lookat.x + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom left position on Grid that is visible by camera
glm::vec2 Camera::GridBottomLeftVisible()
{
	float x = Position.x + lookat.x - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y - ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Bottom right position on Grid that is visible by camera
glm::vec2 Camera::GridBottomRightVisible()
{
	float x = Position.x + lookat.x + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE + ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;
	float y = Position.y + lookat.y + ZoomLevel * 0.5f * ScreenRatio * VISIBLE_RANGE - ZoomLevel * 0.5f * ROOT3 * VISIBLE_RANGE;

	return glm::vec2(x, y);
}

// Current cursor position on Grid
glm::vec2 Camera::CursorPositionOnGrid()
{
	glm::vec2 gridPosition;
	float z = 0.0f;
	float zOld = z;

	gridPosition = cursorPosition(z);

	{
		std::cout << "(x|y) (" << gridPosition.x << "|" << gridPosition.y << ")" << std::endl;

		// Outside of grid cases
		if (gridPosition.y < 0)
			gridPosition.y = 0;
		if (gridPosition.x < 0)
			gridPosition.x = 0;
		if (gridPosition.y > (float)Grid->gridUnits.size() - 1)
			gridPosition.y = (float)Grid->gridUnits.size() - 1;
		if (gridPosition.x > (float)Grid->gridUnits[0].size() - 1)
			gridPosition.x = (float)Grid->gridUnits[0].size() - 1;
	}
	z = Grid->gridUnits[(int)gridPosition.y][(int)gridPosition.x]->averageHeight;

	// Reposition with average height of tile, until coordinates match 
	while (z > zOld + 0.5f || z < zOld - 0.5f) 
	{
		gridPosition.x += z;
		gridPosition.y -= z;
		{
			// Outside of grid cases
			if (gridPosition.y < 0)
				gridPosition.y = 0;
			if (gridPosition.x < 0)
				gridPosition.x = 0;
			if (gridPosition.y > (float)Grid->gridUnits.size() - 1)
				gridPosition.y = (float)Grid->gridUnits.size() - 1;
			if (gridPosition.x > (float)Grid->gridUnits[0].size() - 1)
				gridPosition.x = (float)Grid->gridUnits[0].size() - 1;
		}
		gridPosition = cursorPosition(z);

		zOld = z;
		z = Grid->gridUnits[(int)gridPosition.y][(int)gridPosition.x]->averageHeight;

		std::cout << "Adjusted grid unit" << std::endl;
	}

	std::cout << "(x|y) adjusted (" << gridPosition.x << "|" << gridPosition.y << ") by height z " << z << std::endl;

	return gridPosition;
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

// Calculates (x,y) coordinates on Grid, given a specific height
glm::vec2 Camera::cursorPosition(float z)
{
	int window_width, window_height;
	double window_x, window_y;
	GLfloat x = 0.0f, y = 0.0f;

	glfwGetCursorPos(window, &window_x, &window_y);
	glfwGetWindowSize(window, &window_width, &window_height);


	x += ((float)window_x - window_width / 2); // Cursor offset from middle of screen in X direction
	y += ((float)window_x - window_width / 2);

	x += ((float)window_y - window_height / 2) * ROOT3; // Cursor offset from middle of screen in Y direction
	y -= ((float)window_y - window_height / 2) * ROOT3; // Screen coordinate system starts at top

	x *= (ZoomLevel / window_width) * 2.515f; // Adjust for zoom
	y *= (ZoomLevel / window_width) * 2.515f; // 2.575 = magic number

	x += z;	// Adjust for z height
	y -= z;

	x += Position.x + lookat.x; // Camera offset
	y += Position.y + lookat.y;

	return glm::vec2(x, y);
}
