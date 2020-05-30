#include "stdafx.h"
#include "terrain.h"

Terrain::Terrain(int aGridHeight, int aGridWidth, Grid* aGrid)
{
	gridWidth = aGridWidth;
	gridHeight = aGridHeight;
	grid = aGrid;
}

void Terrain::SetRenderWindow(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{
	/* Check if terrain must be reloaded to GPU */
	if (reloadTerrain || currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		currUpperLeftX = (int)upperLeft.x;
		currUpperLeftY = (int)upperLeft.y;
		currLowerRightX = (int)lowerRight.x;
		currLowerRightY = (int)lowerRight.y;
		reloadTerrain = false;
		LoadVisibleGeometry(upperLeft, upperRight, lowerLeft, lowerRight);
	}
}

void Terrain::Draw(Shader& shader)
{
	int vertexCount = ReloadGPUData();

	// render
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_grass);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id_grass_red);

	glBindVertexArray(VAO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glBindVertexArray(0);
}

int Terrain::ReloadGPUData()
{
	renderDataMutex.lock();
	int vertexCount = -1;
	if (reloadGPUData) // TODO: this is always true?
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if (currRenderData)
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderDataVertexCount * 9, &(*renderData1)[0],
			             GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderDataVertexCount * 9, &(*renderData0)[0],
			             GL_STATIC_DRAW);

		vertexCount = renderDataVertexCount;

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(0);

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// alternative texture attribute
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
	}
	renderDataMutex.unlock();
	return vertexCount;
}

void Terrain::LoadVisibleGeometry(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{
	/* parameters: corners of visible grid on (x/y/z=0) plane */
	/* Create geometry data for visible area */
	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::vector<GLfloat>* renderDataTemp;
	renderDataMutex.lock();
	if (currRenderData)
		renderDataTemp = renderData0;
	else
		renderDataTemp = renderData1;
	renderDataMutex.unlock();

	/* Load GPU data for visible area */
	int index = 0;
	int fromX = std::min(std::min((int)upperLeft.x, int(lowerLeft.x)), std::min((int)upperRight.x, int(lowerRight.x)));
	int endX = std::max(std::max((int)upperLeft.x, int(lowerLeft.x)), std::max((int)upperRight.x, int(lowerRight.x)));
	int fromY = std::min(std::min((int)upperLeft.y, int(lowerLeft.y)), std::min((int)upperRight.y, int(lowerRight.y)));
	int endY = std::max(std::max((int)upperLeft.y, int(lowerLeft.y)), std::max((int)upperRight.y, int(lowerRight.y)));
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	
	for (int i = std::max(0, fromY + 1); i <= std::min(gridHeight - 1, endY); ++i)
	{
		for (int j = std::max(0, fromX + 1); j <= std::min(gridWidth - 1, endX); ++j)
		{
			if (index < maximumVisibleUnits * 54)
			{
				/* Check if the point is inside the rectangle*/
				glm::vec2 AM = glm::vec2(j - upperLeft.x, i - upperLeft.y);
				glm::vec2 AB = glm::vec2(upperRight - upperLeft);
				glm::vec2 AD = glm::vec2(lowerLeft - upperLeft);

				if (0 <= dot(AM, AB) && dot(AM, AB) < dot(AB, AB) &&
					dot(AM, AD) < dot(AD, AD) && 0 <= dot(AM, AD))
				{
					float float_i = float(i);
					float float_j = float(j);
					float occupied;

					if (grid->buildingMode && !grid->IsValidBuildingPosition(j,i,j,i))
						occupied = 1.0f;
					else
						occupied = 0.0f;

					// x/y/z of first vertex
					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j];

					// x/y/z of normal vector
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j].z;

					// texture coord X, Y of first vertex
					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 0.0f;

					// 0 = normal texture, 1.0 = secondary texture, etc.
					(*renderDataTemp)[index++] = occupied;

					
					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j + 1];

					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 0.0f;

					(*renderDataTemp)[index++] = occupied;

					
					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].z;

					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 1.0f;

					(*renderDataTemp)[index++] = occupied;
					
					
					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j].z;

					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 1.0f;

					(*renderDataTemp)[index++] = occupied;

					
					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j + 1];

					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[i * (gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 0.0f;

					(*renderDataTemp)[index++] = occupied;

					
					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j + 1];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1) * (gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 1.0f;

					(*renderDataTemp)[index++] = occupied;
				}
			}
			else
				goto loopExit;
		}
	}
loopExit:
	renderDataMutex.lock();
	currRenderData = !currRenderData;
	renderDataVertexCount = index * 6 / 54;
	reloadGPUData = true;
	renderDataMutex.unlock();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - start;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "LoadVisibleGeometry time elapsed: " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void Terrain::CreateGeometry()
{
	/* Create normals, areas for triangles using explicit cross product formula*/
	triangleNormals = std::vector<glm::vec3>(gridWidth * gridHeight * 2);
	triangleArea = std::vector<float>(gridWidth * gridHeight * 2);
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			/* Left triangle */
			glm::vec3 crossProduct = glm::vec3(-(heightmap[i][j + 1] - heightmap[i][j]),
			                                   heightmap[i + 1][j] - heightmap[i][j],
			                                   1);

			triangleArea[(i * gridWidth + j) * 2] = length(crossProduct) / 2.0f;
			triangleNormals[(i * gridWidth + j) * 2] = normalize(crossProduct);

			/* Right triangle*/
			crossProduct = glm::vec3(-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]) -
			                         (heightmap[i][j + 1] - heightmap[i + 1][j]),
			                         -(heightmap[i + 1][j + 1] - heightmap[i][j + 1]),
			                         1);

			triangleArea[(i * gridWidth + j) * 2 + 1] = length(crossProduct) / 2.0f;
			triangleNormals[(i * gridWidth + j) * 2 + 1] = normalize(crossProduct);
		}
	}

	/* Create vertex normals, currently weighted by area of neighbor triangle not angle (should be similar in this case)*/
	vertexNormals = std::vector<glm::vec3>((gridWidth + 1) * (gridHeight + 1));
	for (int i = 0; i < gridHeight + 1; ++i)
	{
		for (int j = 0; j < gridWidth + 1; ++j)
		{
			// cycle all vertices
			if (i == 0)
			{
				// bottom row
				if (j == 0)
				{
					// bottom left vertex
					vertexNormals[i * (gridWidth + 1) + j] = triangleNormals[0];
				}
				else if (j == gridHeight)
				{
					// bottom right
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(j - 1) * 2] * triangleNormals[(j - 1) * 2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1]);
				}
				else
				{
					// bottom middle
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
							triangleArea[(j - 1) * 2] * triangleNormals[(j - 1) * 2] +
							triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1]) +
						triangleArea[(j - 1) * 2 + 2] * triangleNormals[(j - 1) * 2 + 2];
				}
			}
			else
			{
				if (j == 0 && i < gridHeight)
				{
					// left column, except bottomn left and top left
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(i - 1) * gridWidth * 2] * triangleNormals[(i - 1) * gridWidth * 2] +
						triangleArea[(i - 1) * gridWidth * 2 + 1] * triangleNormals[(i - 1) * gridWidth * 2 + 1] +
						triangleArea[i * gridWidth * 2] * triangleNormals[i * gridWidth * 2]);
				}
				else if (j == 0)
				{
					// top left
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(i - 1) * gridWidth * 2] * triangleNormals[(i - 1) * gridWidth * 2] +
						triangleArea[(i - 1) * gridWidth * 2 + 1] * triangleNormals[(i - 1) * gridWidth * 2 + 1]);
				}
				else if (j == gridWidth && i != gridHeight)
				{
					// right side, middle
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(i - 1) * gridWidth * 2 + j * 2 - 1] * triangleNormals[(i - 1) * gridWidth * 2 + j
							* 2 - 1] +
						triangleArea[i * gridWidth * 2 + j * 2 - 2] * triangleNormals[i * gridWidth * 2 + j * 2 - 2] +
						triangleArea[i * gridWidth * 2 + j * 2 - 1] * triangleNormals[i * gridWidth * 2 + j * 2 - 1]);
				}
				else if (j == gridWidth && i == gridHeight)
				{
					// right top
					vertexNormals[i * (gridWidth + 1) + j] = triangleNormals[gridWidth * gridHeight - 1];
				}
				else if (i == gridHeight)
				{
					// top row, except left and right
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(i - 1) * gridWidth * 2 + j * 2 - 1] * triangleNormals[(i - 1) * gridWidth * 2 +
							j * 2 - 1] +
						triangleArea[(i - 1) * gridWidth * 2 + j * 2] * triangleNormals[(i - 1) * gridWidth * 2 + j *
							2] +
						triangleArea[(i - 1) * gridWidth * 2 + j * 2 + 1] * triangleNormals[(i - 1) * gridWidth * 2 +
							j * 2 + 1]);
				}
				else
				{
					// rest, all in the middle with 6 surrounding triangles
					vertexNormals[i * (gridWidth + 1) + j] = normalize(
						triangleArea[(i - 1) * gridWidth * 2 + j * 2 - 1] * triangleNormals[(i - 1) * gridWidth * 2 +
							j * 2 - 1] +
						triangleArea[(i - 1) * gridWidth * 2 + j * 2] * triangleNormals[(i - 1) * gridWidth * 2 + j *
							2] +
						triangleArea[(i - 1) * gridWidth * 2 + j * 2 + 1] * triangleNormals[(i - 1) * gridWidth * 2 +
							j * 2 + 1] +
						triangleArea[i * gridWidth * 2 + j * 2 - 2] * triangleNormals[i * gridWidth * 2 + j * 2 - 2]
						+
						triangleArea[i * gridWidth * 2 + j * 2 - 1] * triangleNormals[i * gridWidth * 2 + j * 2 - 1]
						+
						triangleArea[i * gridWidth * 2 + j * 2] * triangleNormals[i * gridWidth * 2 + j * 2 + 1]);
				}
			}
		}
	}
}

void Terrain::Accept(Visitor& v)
{
	v.Visit(this);
}

void Terrain::InitOpenGL()
{
	texture_id_grass = grass.TextureFromFile(Path + "/../terrain/" + texture_grass.c_str());
	texture_id_grass_red = grass.TextureFromFile(Path + "/../terrain/" + texture_grass_red.c_str());
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

Terrain::~Terrain()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	delete renderData0;
	delete renderData1;
}
