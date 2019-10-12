#include "stdafx.h"
#pragma once
#include "camera.h"

Camera::Camera(const glm::vec3 position, GLFWwindow* window)
{
	Position = position;
	Window = window;
	MaxX = (int)MAP_WIDTH - 1;
	MaxY = (int)MAP_HEIGHT - 1;

	VecUp = glm::vec3(-1.0f, 1.0f, 0.0f);
	VecRight = glm::vec3(1.0f, 1.0f, 0.0f);
	LookAt = glm::vec3(-50.0f, 50.0f, -50.0f);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return lookAt(Position, Position + LookAt, VecUp);
}

glm::mat4 Camera::GetLightViewMatrix() const
{
	return lookAt(Position + LookAt + DirectionalLight.PositionOffset, Position + LookAt, VecUp);
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

glm::vec2 Camera::GridTopLeftVisible() const
{
	return glm::vec2(TopLeftVisible.first, TopLeftVisible.second);
}

glm::vec2 Camera::GridTopRightVisible() const
{
	return glm::vec2(TopRightVisible.first, TopRightVisible.second);
}

glm::vec2 Camera::GridBottomLeftVisible() const
{
	return glm::vec2(BottomLeftVisible.first, BottomLeftVisible.second);
}

glm::vec2 Camera::GridBottomRightVisible() const
{
	return glm::vec2(BottomRightVisible.first, BottomRightVisible.second);
}

glm::vec3 Camera::CursorPositionOnGrid() const
{
	double windowX, windowY;
	glm::vec2 window;

	glfwGetCursorPos(Window, &windowX, &windowY);
	window.x = (float)windowX;
	window.y = (float)windowY;

	return PixelTo3DCoordinate(window);
}

void Camera::Scroll(CameraMovement direction, float yOffset)
{
	if (direction == Up)
		Position += VecUp * yOffset * ZoomLevel * ROOT3;
	if (direction == Down)
		Position -= VecUp * yOffset * ZoomLevel * ROOT3;
	if (direction == Left)
		Position -= VecRight * yOffset * ZoomLevel;
	if (direction == Right)
		Position += VecRight * yOffset * ZoomLevel;
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
	DirectionalLight.Direction = -DirectionalLight.PositionOffset;
}

void Camera::CalculateVisibleGrid()
{
	const int xLookat = (int)(Position.x + LookAt.x);
	const int yLookat = (int)(Position.y + LookAt.y);
	
	float z;
	

	// Find left edge
	TopLeftVisible = std::make_pair(xLookat, yLookat);
	do
	{
		TopLeftVisible.first--;
		TopLeftVisible.second--;
		if (TopLeftVisible.first >= 0 &&
			TopLeftVisible.first <= MaxX &&
			TopLeftVisible.second >= 0 &&
			TopLeftVisible.second <= MaxY)
		{
			z = Grid->GetHeight((float)TopLeftVisible.first, (float)TopLeftVisible.second);
		} else 
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)TopLeftVisible.first,(float)TopLeftVisible.second,z)));
	BottomLeftVisible = TopLeftVisible;

	
	// Find right edge
	TopRightVisible = std::make_pair(xLookat, yLookat);
	do
	{
		TopRightVisible.first++;
		TopRightVisible.second++;
		if (TopRightVisible.first >= 0 &&
			TopRightVisible.first <= MaxX &&
			TopRightVisible.second >= 0 &&
			TopRightVisible.second <= MaxY)
		{
			z = Grid->GetHeight((float)TopRightVisible.first, (float)TopRightVisible.second);
		} else 
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)TopRightVisible.first,(float)TopRightVisible.second,z)));
	BottomRightVisible = TopRightVisible;

	
	/* Increase top edge until all grid points are outside of view */
	// Move x/y along the left edge to the top, until out of vision
	int x = TopLeftVisible.first + 1;
	int y = TopLeftVisible.second + 1;
	do
	{
		x--;
		y++;
		if (x >= 0 &&
			x <= MaxX &&
			y >= 0 &&
			y <= MaxY)
		{
			z = Grid->GetHeight((float)x, (float)y);
		} else
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)x,(float)y,z)));
	TopLeftVisible.first = x - 2;
	TopLeftVisible.second = y - 2;
	
	// Move x/y along the top edge, until in vision again, if in vision, move one higher
	do
	{
		x++;
		y++;
		if (x >= 0 &&
			x <= MaxX &&
			y >= 0 &&
			y <= MaxY)
		{
			z = Grid->GetHeight((float)x, (float)y);
		} else
		{
			z = 0;
		}
		if (CoordinateVisible(glm::vec3((float)x, (float)y, z)))
		{
			x--;
			y++;
			TopLeftVisible.first--;
			TopLeftVisible.second++;
		}
	} while (TopRightVisible.first + TopRightVisible.second > x + y);
	// When all the way to top right, set TopRightVisible to current x/y
	TopRightVisible = std::make_pair(x, y);

	
	/* Decrease bottom edge until all grid points are outside of view */
	// Move x/y along the left edge to the bottom, until out of vision
	x = BottomLeftVisible.first + 1;
	y = BottomLeftVisible.second + 1;
	do
	{
		x++;
		y--;
		if (x >= 0 &&
			x <= MaxX &&
			y >= 0 &&
			y <= MaxY)
		{
			z = Grid->GetHeight( (float)x, (float)y);
		} else
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)x,(float)y,z)));
	BottomLeftVisible.first = x - 2;
	BottomLeftVisible.second = y - 2;

	// Move x/y along the bottom edge, until in vision again, if in vision, move one lower
	do
	{
		x++;
		y++;
		if (x >= 0 &&
			x <= MaxX &&
			y >= 0 &&
			y <= MaxY)
		{
			z = Grid->GetHeight((float)x, (float)y);
		} else
		{
			z = 0;
		}
		if (CoordinateVisible(glm::vec3((float)x, (float)y, z)))
		{
			x++;
			y--;
			BottomLeftVisible.first++;
			BottomLeftVisible.second--;
		}
	} while (BottomRightVisible.first + BottomRightVisible.second > x + y);
	// When all the way to top right, set BottomRightVisible to current x/y
	BottomRightVisible = std::make_pair(x, y);

	BottomLeftVisible.first++;
	BottomLeftVisible.second--;

	BottomRightVisible.first++;
	BottomRightVisible.second--;
}

glm::vec3 Camera::PixelTo3DCoordinate(glm::vec2 pixelCoordinates) const
{
	float z;

	glReadPixels((int)pixelCoordinates.x, (int)((float)ScreenHeight - pixelCoordinates.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	return PixelTo3DCoordinate(pixelCoordinates, z);
}

glm::vec3 Camera::PixelTo3DCoordinate(glm::vec2 pixelCoordinates, float z) const
{
	const glm::vec3 window = glm::vec3(pixelCoordinates.x, (float)ScreenHeight - pixelCoordinates.y, z);
	const glm::mat4 model = GetViewMatrix();
	const glm::mat4 projection = GetProjectionMatrix();
	const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, (float)ScreenWidth, (float)ScreenHeight);

	return unProject(window, model, projection, viewport);
}

bool Camera::CoordinateVisible(glm::vec3 coordinate) const
{
	const glm::vec4 pixel = GetProjectionMatrix() * GetViewMatrix() * glm::vec4(coordinate, 1.0f);
	
	return (pixel.x > -1 &&
			pixel.x < 1 &&
			pixel.y > -1 &&
			pixel.y < 1);
}
