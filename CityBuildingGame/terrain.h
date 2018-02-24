#include <vector>
#include <algorithm>
#include <time.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Include GLM, math library
#include "glm/glm.hpp"
#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

#include "common.h"
#include "heightmap.h"
#include "shader.h"

using namespace std;

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	float averageHeight = 0;
	
	Unit(float pAverageHeight);
	~Unit();
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	void Initialize(int argWidth, int argHeight);
	float GetHeight(int argX, int argY);
	void Draw();

	vector<vector<Unit>> grid;
	vector<vector<float>> heightmap;

private:
	void CreateGeometry();
	void LoadVisibleGeometry();
	void LoadVisibleGeometry(int startX, int endX, int startY, int endY);
	void LoadTextures();
	void CreateGrid();
	void AddTexturesToGrid();
	void PopulateGridWithObjects();

	GLuint VBO, VAO, EBO;
	unsigned int texture_id_grass;
	string texture_grass = "Grass.bmp";
	
	int gridHeight;
	int gridWidth;

	vector<glm::vec3> vertexNormals;
	vector<glm::vec3> triangleNormals;
	vector<float> triangleArea;

	/* Defines what is rendered of the terrain*/
	int visibleHeight;
	int visibleWidth;
	int currStartX, currEndX, currStartY, currEndY = 0;

	vector<GLfloat> renderData; /* Gets sent to GPU */
	vector<unsigned int> indices;
};