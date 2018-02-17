/* 
Keep a grid of squares of the complete terrain
Create VBO of visible part of terrain, send to GPU on load (resend if terrain changes or siginificant scrolling).

*/
#include "terrain.h"

const unsigned int shaderAttribute = 0;
GLuint VBO;

Terrain::Terrain() {}

void Terrain::Initialize(int aWidth, int aHeight)
{
	height = aHeight;
	width = aWidth;

	for(int i = 0; i < height; i++) // Create new array
	{
		heightMap.push_back(vector<float>(width));
		for(int j = 0; j < width; j++)
		{
			heightMap.back().at(j) = 0;
		}
	}

	// Fill array with noise
	GenerateWhiteNoise();
	GeneratePerlinNoise(6);

	// Create VBO for faster rendering
	//LoadTextures();
	CreateGeometry();
}
Terrain::~Terrain()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
void Terrain::GenerateWhiteNoise()
{ 
	srand((unsigned)time(0)); // seed random numbers

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
			heightMap[i][j] = (float)rand()/(float)RAND_MAX; // generate random numbers between 0 and 1
        }
    }
}
vector<vector<float>> Terrain::GenerateSmoothNoise(vector<vector<float>> baseNoise, int octave)
{
	// start with empty array
	vector<vector<float>> smoothNoise;
	for(int i = 0; i < height; i++)
	{
		smoothNoise.push_back(vector<float>(width));
		for(int j = 0; j < width; j++)
		{
			smoothNoise.back().at(j) = 0;
		}
	}

    int samplePeriod = 1 << octave; // calculates 2 ^ k
    float sampleFrequency = 1.0f / samplePeriod;
 
    for (int i = 0; i < height; i++)
    {
		//calculate the horizontal sampling indices
        int sample_i0 = (i / samplePeriod) * samplePeriod;
        int sample_i1 = (sample_i0 + samplePeriod) % height; //wrap around
        float vertical_blend = (i - sample_i0) * sampleFrequency;
 
        for (int j = 0; j < width; j++)
        {
			//calculate the vertical sampling indices
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % width; //wrap around
			float horizontal_blend = (j - sample_j0) * sampleFrequency;
 
			//blend the top two corners
			float top = Interpolate(baseNoise[sample_i0][sample_j0],
			baseNoise[sample_i1][sample_j0], vertical_blend);
 
			//blend the bottom two corners
			float bottom = Interpolate(baseNoise[sample_i0][sample_j1],
			baseNoise[sample_i1][sample_j1], vertical_blend);
 
			//final blend
			smoothNoise[i][j] = Interpolate(top, bottom, horizontal_blend);
		}
    }
	return smoothNoise;
}
float Terrain::Interpolate(float x0, float x1, float alpha)
{
   return x0 * (1 - alpha) + alpha * x1;
}
void Terrain::GeneratePerlinNoise(int octaveCount)
{
	vector<vector<vector<float>>> smoothNoiseList; //an array of 2D arrays containing

	float persistence = 0.5f;

    //generate smooth noise
    for (int i = 0; i < octaveCount; i++)
    {
	    smoothNoiseList.push_back(GenerateSmoothNoise(heightMap, i));
    }

	float amplitude = 1.0f;
	float totalAmplitude = 0.0f;

    //blend noise together
	for (auto &i : heightMap)
		std::fill(i.begin(), i.end(), 0);

	float maxValue = 0;
    for (int octave = octaveCount - 1; octave >= 0; octave--)
    {
		amplitude *= persistence;
		totalAmplitude += amplitude;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				heightMap[i][j] += smoothNoiseList[octave][i][j] * amplitude;
			}
		}       
    }
 
   //normalisation
   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
      {
         heightMap[i][j] /= totalAmplitude;
      }
   }
}
float Terrain::GetHeight(int argX, int argY)
{
	return heightMap[argY][argX];
}
void Terrain::Draw()
{
	// render
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);*/

	glBindVertexArray(VAO);

	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f);
	ourShader.setMat4("model", model);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, width*height*6);
}
void Terrain::CreateGeometry()
{
	float max_height = 20.0f;

	vector<GLfloat> terrainVector;
	vector<unsigned int> indices;

	int index = 0;
	for(int i = 0; i < height -1; i++)
	{
		for(int j = 0; j < width -1; j++)
		{
			terrainVector.push_back(j);
			terrainVector.push_back(i); 
			terrainVector.push_back(heightMap[j][i]* max_height);

			// texture 
			terrainVector.push_back(0.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j + 1);
			terrainVector.push_back(i);
			terrainVector.push_back(heightMap[j + 1][i]* max_height);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j);
			terrainVector.push_back(i + 1);
			terrainVector.push_back(heightMap[j][i + 1]* max_height);

			terrainVector.push_back(0.0f);
			terrainVector.push_back(1.0f);

			terrainVector.push_back(j); 
			terrainVector.push_back(i + 1); 
			terrainVector.push_back(heightMap[j][i + 1]* max_height);

			terrainVector.push_back(0.0f);
			terrainVector.push_back(1.0f);

			terrainVector.push_back(j + 1);
			terrainVector.push_back(i);
			terrainVector.push_back(heightMap[j + 1][i]* max_height);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j+1);
			terrainVector.push_back(i+1);
			terrainVector.push_back(heightMap[j + 1][i + 1]* max_height);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(1.0f);

			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
		}
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * terrainVector.size(), &terrainVector[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Terrain::LoadTextures()
{
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int tex_width, tex_height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(texture_grass_path.c_str(), &tex_width, &tex_height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
}