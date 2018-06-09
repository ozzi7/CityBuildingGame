#include "terrain.h"

Terrain::Terrain(int aGridHeight, int aGridWidth){
	gridWidth = aGridWidth;
	gridHeight = aGridHeight;

	NoiseGen noise_gen;
	heightmap = vector<vector<float>>(gridHeight + 1, vector<float>(gridWidth + 1, 0));
	noise_gen.GeneratePerlinNoise(heightmap, gridHeight + 1, gridWidth + 1, -2.5f, 2.5f, 6);

	CreateGeometry();

	renderData0 = new vector<GLfloat>(maximumVisibleUnits* 48);
	renderData1 = new vector<GLfloat>(maximumVisibleUnits* 48);
}

void Terrain::SetRenderWindow(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{
	/* Check if terrain must be reloaded to GPU */
	if (currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		currUpperLeftX = upperLeft.x;
		currUpperLeftY = upperLeft.y;
		currLowerRightX = lowerRight.x;
		currLowerRightY = lowerRight.y;
		LoadVisibleGeometry(upperLeft, upperRight, lowerLeft, lowerRight);
	}
}
void Terrain::AddTexturesToGrid()
{

}
void Terrain::PopulateGridWithObjects()
{

}
void Terrain::Draw(Shader &shaderTerrain)
{
	int vertexCount = ReloadGPUData();

	// render
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_grass);

	glBindVertexArray(VAO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}
int Terrain::ReloadGPUData()
{
	renderDataMutex.lock();
	int vertexCount = -1;
	if (reloadGPUData)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if (currRenderData)
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderDataVertexCount *8, &(*renderData1)[0], GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderDataVertexCount *8, &(*renderData0)[0], GL_STATIC_DRAW);

		vertexCount = renderDataVertexCount;

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	renderDataMutex.unlock();
	return vertexCount;
}
void Terrain::LoadVisibleGeometry(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{	
	/* parameters: corners of visible grid on (x/y/z=0) plane */
	/* Create geometry data for visible area */

	vector<GLfloat> *renderDataTemp;
	renderDataMutex.lock();
	if (currRenderData)
		renderDataTemp = renderData0;
	else
		renderDataTemp = renderData1;
	renderDataMutex.unlock();

	/* Load GPU data for visible area */
	int index = 0;
	int startX = min(min((int)upperLeft.x, int(lowerLeft.x)), min((int)upperRight.x, int(lowerRight.x)));
	int endX = max(max((int)upperLeft.x, int(lowerLeft.x)), max((int)upperRight.x, int(lowerRight.x)));
	int startY = min(min((int)upperLeft.y, int(lowerLeft.y)), min((int)upperRight.y, int(lowerRight.y)));
	int endY = max(max((int)upperLeft.y, int(lowerLeft.y)), max((int)upperRight.y, int(lowerRight.y)));

	for (int i = max(0, startY+1); i <= min(gridHeight-1, endY); ++i)
	{
		for (int j = max(0, startX+1); j <= min(gridWidth-1, endX); ++j)
		{
			if (index < maximumVisibleUnits*48)
			{
				/* Check if the point is inside the rectangle*/
				glm::vec2 AM = glm::vec2(j - upperLeft.x, i - upperLeft.y);
				glm::vec2 AB = glm::vec2(upperRight - upperLeft);
				glm::vec2 AD = glm::vec2(lowerLeft - upperLeft);

				if ((0 <= glm::dot(AM, AB)) && (glm::dot(AM, AB) < glm::dot(AB, AB)) &&
					(glm::dot(AM, AD) < glm::dot(AD, AD)) && 0 <= glm::dot(AM, AD))
				{
					float float_i = float(i);
					float float_j = float(j);
					// x/y/z of first vertex
					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j];

					// x/y/z of normal vector
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j].z;

					// texture coord X, Y of first vertex
					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 0.0f;

					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j + 1];

					// x/y/z of normal vector
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 0.0f;

					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].z;

					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 1.0f;

					(*renderDataTemp)[index++] = float_j;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j].z;

					(*renderDataTemp)[index++] = 0.0f;
					(*renderDataTemp)[index++] = 1.0f;

					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i;
					(*renderDataTemp)[index++] = heightmap[i][j + 1];

					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[i*(gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 0.0f;

					(*renderDataTemp)[index++] = float_j + 1;
					(*renderDataTemp)[index++] = float_i + 1;
					(*renderDataTemp)[index++] = heightmap[i + 1][j + 1];

					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j + 1].x;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j + 1].y;
					(*renderDataTemp)[index++] = vertexNormals[(i + 1)*(gridWidth + 1) + j + 1].z;

					(*renderDataTemp)[index++] = 1.0f;
					(*renderDataTemp)[index++] = 1.0f;
				}
			}
			else
				goto loopExit;
		}
	}
	loopExit:
	renderDataMutex.lock();
	if (currRenderData == 1)
	{
		currRenderData = 0;
	}
	else
	{
		currRenderData = 1;
	}
	renderDataVertexCount = (index / 48) * 6;
	reloadGPUData = true;
	renderDataMutex.unlock();
}
void Terrain::CreateGeometry()
{
	/* Create normals, areas for triangles using explicit cross product formula*/
	triangleNormals = vector<glm::vec3>(gridWidth*gridHeight * 2);
	triangleArea = vector<float>(gridWidth*gridHeight * 2);
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			/* Left triangle */
			glm::vec3 crossProduct = glm::vec3(-(heightmap[i][j + 1] - heightmap[i][j]),
				heightmap[i + 1][j] - heightmap[i][j],
				1);

			triangleArea[(i*gridWidth + j) * 2] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2] = glm::normalize(crossProduct);

			/* Right triangle*/
			crossProduct = glm::vec3(-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]) -
				(heightmap[i][j + 1] - heightmap[i + 1][j]),
				-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]),
				1);

			triangleArea[(i*gridWidth + j) * 2 + 1] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2 + 1] = glm::normalize(crossProduct);
		}
	}

	/* Create vertex normals, currently weighted by area of neighbor triangle not angle (should be similar in this case)*/
	vertexNormals = vector<glm::vec3>((gridWidth+1)*(gridHeight+1));
	for (int i = 0; i < gridHeight + 1; ++i)
	{
		for (int j = 0; j < gridWidth + 1; ++j)
		{
			// cycle all vertices
			if (i == 0) {
				// bottom row
				if (j == 0) {
					// bottom left vertex
					vertexNormals[i*(gridWidth+1)+j] = triangleNormals[0];
				} else if (j == gridHeight) {
					// bottom right
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(j - 1) * 2] * triangleNormals[(j - 1) * 2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1]);
				}
				else {
					// bottom middle
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(j - 1) * 2]*triangleNormals[(j-1)*2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1])+
						triangleArea[(j - 1) * 2 + 2] * triangleNormals[(j - 1) * 2 + 2];
				}
			}
			else {
				if (j == 0 && i < gridHeight) {
					// left column, except bottomn left and top left
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i-1)*gridWidth*2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2+1] * triangleNormals[(i - 1)*gridWidth * 2+1] +
						triangleArea[i*gridWidth * 2] * triangleNormals[i * gridWidth*2]);
				}
				else if(j == 0) {
					// top left
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2 + 1] * triangleNormals[(i - 1)*gridWidth * 2 + 1]);
				}
				else if (j == gridWidth && i != gridHeight) {
					// right side, middle 
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 +j*2 - 1] * triangleNormals[(i - 1)*gridWidth * 2 + j * 2 - 1] +
						triangleArea[i *gridWidth * 2 + j * 2 - 2] * triangleNormals[i *gridWidth * 2 + j * 2 - 2] +
						triangleArea[i *gridWidth * 2 + j * 2 - 1] * triangleNormals[i *gridWidth * 2 + j * 2 - 1]);
				}
				else if (j == gridWidth && i == gridHeight) {
					// right top 
					vertexNormals[i*(gridWidth + 1) + j] = triangleNormals[gridWidth*gridHeight-1];
				}
				else if (i == gridHeight) {
					// top row, except left and right 
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 + (j*2)-1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)+1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]);
				}
				else
				{
					// rest, all in the middle with 6 surrounding triangles
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 + (j * 2) - 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2) + 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]+
						triangleArea[i*gridWidth * 2 + (j * 2) - 2] * triangleNormals[i*gridWidth * 2 + (j * 2) - 2] +
						triangleArea[i*gridWidth * 2 + (j * 2) - 1] * triangleNormals[i*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[i*gridWidth * 2 + (j * 2)]  * triangleNormals[i*gridWidth * 2 + (j * 2) + 1]);
				}
			}
		}
	}
}
void Terrain::Accept(Visitor &v)
{
	v.Visit(this);
}
void Terrain::InitOpenGL(Shader* terrain_shader, string exe_path)
{
	terrain_shader->use();
	LoadTextures(terrain_shader, exe_path);
	GenerateBuffers();
}
void Terrain::LoadTextures(Shader * terrain_shader, string exe_path)
{
	Model tree = Model();
	string texturesPath = exe_path + "/../terrain";
	texture_id_grass = tree.TextureFromFile(texture_grass.c_str(), texturesPath);

	terrain_shader->setInt("texture1", 0);
}
void Terrain::GenerateBuffers()
{
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