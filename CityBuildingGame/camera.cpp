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

LightSource Camera::GetDirectionalLight() const
{
	return DirectionalLight;
}

void Camera::SetDirectionalLightPositionOffset(glm::vec3 coordinateOffset)
{
	DirectionalLight.PositionOffset = coordinateOffset;
	DirectionalLight.Direction = -DirectionalLight.PositionOffset;
	CalculateLightProjectionMatrix();
}

void Camera::SetDirectionalLightColor(glm::vec3 color)
{
	DirectionalLight.Color = color;
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
	return LightProjectionMatrix;
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
	if (direction == CameraMovement::Up)
		Position += VecUp * yOffset * ZoomLevel * ROOT3;
	if (direction == CameraMovement::Down)
		Position -= VecUp * yOffset * ZoomLevel * ROOT3;
	if (direction == CameraMovement::Left)
		Position -= VecRight * yOffset * ZoomLevel;
	if (direction == CameraMovement::Right)
		Position += VecRight * yOffset * ZoomLevel;
	CalculateVisibleGrid();
	CalculateLightProjectionMatrix();
}

void Camera::Zoom(float yOffset)
{
	if (ZoomLevel >= ZOOM_MIN && ZoomLevel <= ZOOM_MAX)
		ZoomLevel -= yOffset * ZoomLevel / 10.0f;
	if (ZoomLevel <= ZOOM_MIN)
		ZoomLevel = ZOOM_MIN;
	if (ZoomLevel >= ZOOM_MAX)
		ZoomLevel = ZOOM_MAX;
	CalculateVisibleGrid();
	CalculateLightProjectionMatrix();
}

void Camera::CalculateVisibleGrid()
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::NOTSET, "CalculateVisibleGrid called"));
	const int xLookat = (int)(Position.x + LookAt.x);
	const int yLookat = (int)(Position.y + LookAt.y);
	
	float z; 
	

	// Find left edge
	std::pair<int, int> topLeftVisible = std::make_pair(xLookat, yLookat);
	do
	{
		topLeftVisible.first--;
		topLeftVisible.second--;
		if (topLeftVisible.first >= 0 &&
			topLeftVisible.first <= MaxX &&
			topLeftVisible.second >= 0 &&
			topLeftVisible.second <= MaxY)
		{
			z = Grid->GetHeight((float)topLeftVisible.first, (float)topLeftVisible.second);
		} else 
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)topLeftVisible.first,(float)topLeftVisible.second,z)));

	std::pair<int, int> bottomLeftVisible = topLeftVisible;

	
	// Find right edge
	std::pair<int, int> topRightVisible = std::make_pair(xLookat, yLookat);
	do
	{
		topRightVisible.first++;
		topRightVisible.second++;
		if (topRightVisible.first >= 0 &&
			topRightVisible.first <= MaxX &&
			topRightVisible.second >= 0 &&
			topRightVisible.second <= MaxY)
		{
			z = Grid->GetHeight((float)topRightVisible.first, (float)topRightVisible.second);
		} else 
		{
			z = 0;
		}
	} while (CoordinateVisible(glm::vec3((float)topRightVisible.first,(float)topRightVisible.second,z)));

	std::pair<int, int> bottomRightVisible = topRightVisible;

	
	/* Increase top edge until all grid points are outside of view */
	// Move x/y along the left edge to the top, until out of vision
	int x = topLeftVisible.first + 1;
	int y = topLeftVisible.second + 1;
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
	topLeftVisible.first = x - 2;
	topLeftVisible.second = y - 2;
	
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
			topLeftVisible.first--;
			topLeftVisible.second++;
		}
	} while (topRightVisible.first + topRightVisible.second > x + y);
	// When all the way to top right, set topRightVisible to current x/y
	topRightVisible = std::make_pair(x, y);

	
	/* Decrease bottom edge until all grid points are outside of view */
	// Move x/y along the left edge to the bottom, until out of vision
	x = bottomLeftVisible.first + 1;
	y = bottomLeftVisible.second + 1;
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
	bottomLeftVisible.first = x - 2;
	bottomLeftVisible.second = y - 2;

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
			bottomLeftVisible.first++;
			bottomLeftVisible.second--;
		}
	} while (bottomRightVisible.first + bottomRightVisible.second > x + y);
	// When all the way to top right, set bottomRightVisible to current x/y
	bottomRightVisible = std::make_pair(x, y);

	bottomLeftVisible.first++;
	bottomLeftVisible.second--;

	bottomRightVisible.first++;
	bottomRightVisible.second--;


	TopLeftVisible = topLeftVisible;
	TopRightVisible = topRightVisible;
	BottomLeftVisible = bottomLeftVisible;
	BottomRightVisible = bottomRightVisible;
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
	return CoordinateVisible(coordinate, GetProjectionMatrix(), GetViewMatrix());
}

bool Camera::CoordinateVisible(glm::vec3 coordinate, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	const glm::vec4 pixel = projectionMatrix * viewMatrix * glm::vec4(coordinate, 1.0f);
	
	return (pixel.x > -1 &&
			pixel.x < 1 &&
			pixel.y > -1 &&
			pixel.y < 1);
}

void Camera::CalculateLightProjectionMatrix()
{
	glm::mat4 lightProjectionMatrix = glm::ortho(-ZoomLevel, ZoomLevel,-1.0f * ZoomLevel, ZoomLevel, -100.0f, 100.0f);
	float heightTopRight;
	float heightTopLeft;
	float heightBottomRight;
	float heightBottomLeft;
	float projectionIncrease = 0.5f;

	if (TopRightVisible.first > MaxX || TopRightVisible.second > MaxY ||
		TopRightVisible.first <= 0 || TopRightVisible.second <= 0)
	{
		heightTopRight = 0.0f;
	} else
		heightTopRight = Grid->GetHeight((float)TopRightVisible.first + 0.5f, (float)TopRightVisible.second + 0.5f);

	if (TopLeftVisible.first > MaxX || TopLeftVisible.second > MaxY ||
		TopLeftVisible.first <= 0 || TopLeftVisible.second <= 0)
	{
		heightTopLeft = 0.0f;
	} else
		heightTopLeft = Grid->GetHeight((float)TopLeftVisible.first + 0.5f, (float)TopLeftVisible.second + 0.5f);

	if (BottomRightVisible.first > MaxX || BottomRightVisible.second > MaxY ||
		BottomRightVisible.first <= 0 || BottomRightVisible.second <= 0)
	{
		heightBottomRight = 0.0f;
	} else
		heightBottomRight = Grid->GetHeight((float)BottomRightVisible.first + 0.5f, (float)BottomRightVisible.second + 0.5f);

	if (BottomLeftVisible.first > MaxX || BottomLeftVisible.second > MaxY ||
		BottomLeftVisible.first <= 0 || BottomLeftVisible.second <= 0)
	{
		heightBottomLeft = 0.0f;
	} else
		heightBottomLeft = Grid->GetHeight((float)BottomLeftVisible.first + 0.5f, (float)BottomLeftVisible.second + 0.5f);
	

	/* Increase projection until everything is in field of view */

	while (!(CoordinateVisible(glm::vec3(TopRightVisible.first, TopRightVisible.second, heightTopRight), lightProjectionMatrix, GetLightViewMatrix()) &&
			 CoordinateVisible(glm::vec3(TopLeftVisible.first, TopLeftVisible.second, heightTopLeft), lightProjectionMatrix, GetLightViewMatrix()) &&
			 CoordinateVisible(glm::vec3(BottomRightVisible.first, BottomRightVisible.second, heightBottomRight), lightProjectionMatrix, GetLightViewMatrix()) &&
			 CoordinateVisible(glm::vec3(BottomLeftVisible.first, BottomLeftVisible.second, heightBottomLeft), lightProjectionMatrix, GetLightViewMatrix())))
	{
		projectionIncrease *= 1.5f;
		lightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncrease, ZoomLevel * projectionIncrease, -1.0f * ZoomLevel * projectionIncrease,  ZoomLevel * projectionIncrease, -100.0f, 100.0f);
		if (projectionIncrease > 1000.0f) { break; } // This shouldn't happen, but just in case to prevent infinite loop
	}


	/* Decrease each edge until something isn't in field of view anymore, then increase by one step to get it back into field of view */
	projectionIncrease *= 1.5f;
	float projectionIncreaseA = projectionIncrease;
	float projectionIncreaseB = projectionIncrease;
	float projectionIncreaseC = projectionIncrease;
	float projectionIncreaseD = projectionIncrease;

	LightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB* 0.5f, -1.0f * ZoomLevel * projectionIncreaseC* 0.5f,  ZoomLevel * projectionIncreaseD * 0.5f, -100.0f, 100.0f);
	return;
	
	while (CoordinateVisible(glm::vec3(TopRightVisible.first, TopRightVisible.second, heightTopRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(TopLeftVisible.first, TopLeftVisible.second, heightTopLeft), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomRightVisible.first, BottomRightVisible.second, heightBottomRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomLeftVisible.first, BottomLeftVisible.second, heightBottomLeft), lightProjectionMatrix, GetLightViewMatrix()))
	{
		projectionIncreaseA *= 0.9f;
		lightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB, -1.0f * ZoomLevel * projectionIncreaseC,  ZoomLevel * projectionIncreaseD, -100.0f, 100.0f);
		if (projectionIncreaseA < 0.01f) { break; } // This shouldn't happen, but just in case to prevent infinite loop
	}
	projectionIncreaseA *= 1.11112f;

	while (CoordinateVisible(glm::vec3(TopRightVisible.first, TopRightVisible.second, heightTopRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(TopLeftVisible.first, TopLeftVisible.second, heightTopLeft), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomRightVisible.first, BottomRightVisible.second, heightBottomRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomLeftVisible.first, BottomLeftVisible.second, heightBottomLeft), lightProjectionMatrix, GetLightViewMatrix()))
	{
		projectionIncreaseB *= 0.9f;
		lightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB, -1.0f * ZoomLevel * projectionIncreaseC,  ZoomLevel * projectionIncreaseD, -100.0f, 100.0f);
		if (projectionIncreaseB < 0.01f) { break; } // This shouldn't happen, but just in case to prevent infinite loop
	}
	projectionIncreaseB *= 1.11112f;

	while (CoordinateVisible(glm::vec3(TopRightVisible.first, TopRightVisible.second, heightTopRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(TopLeftVisible.first, TopLeftVisible.second, heightTopLeft), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomRightVisible.first, BottomRightVisible.second, heightBottomRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomLeftVisible.first, BottomLeftVisible.second, heightBottomLeft), lightProjectionMatrix, GetLightViewMatrix()))
	{
		projectionIncreaseC *= 0.9f;
		lightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB, -1.0f * ZoomLevel * projectionIncreaseC,  ZoomLevel * projectionIncreaseD, -100.0f, 100.0f);
		if (projectionIncreaseC < 0.01f) { break; } // This shouldn't happen, but just in case to prevent infinite loop
	}
	projectionIncreaseC *= 1.11112f;

	while (CoordinateVisible(glm::vec3(TopRightVisible.first, TopRightVisible.second, heightTopRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(TopLeftVisible.first, TopLeftVisible.second, heightTopLeft), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomRightVisible.first, BottomRightVisible.second, heightBottomRight), lightProjectionMatrix, GetLightViewMatrix()) &&
		   CoordinateVisible(glm::vec3(BottomLeftVisible.first, BottomLeftVisible.second, heightBottomLeft), lightProjectionMatrix, GetLightViewMatrix()))
	{
		projectionIncreaseD *= 0.9f;
		lightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB, -1.0f * ZoomLevel * projectionIncreaseC,  ZoomLevel * projectionIncreaseD, -100.0f, 100.0f);
		if (projectionIncreaseD < 0.01f) { break; } // This shouldn't happen, but just in case to prevent infinite loop
	}
	projectionIncreaseD *= 1.11112f;

	LightProjectionMatrix = glm::ortho(-ZoomLevel * projectionIncreaseA, ZoomLevel * projectionIncreaseB* 0.5f, -1.0f * ZoomLevel * projectionIncreaseC* 0.5f,  ZoomLevel * projectionIncreaseD * 0.5f, -100.0f, 100.0f);
}
