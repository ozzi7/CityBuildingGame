#include "game_class.h"

GameClass::GameClass(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight)
{
	screenWidth = aScreenWidth;
	screenHeight = aScreenHeight;

	//// Generate terrain with random noise
	terrain.Initialize(aMapWidth, aMapHeight);
}

void GameClass::InitCamera()
{
	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);


	//glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	//// Model matrix : an identity matrix (model will be at the origin)
	//glm::mat4 Model = glm::mat4(1.0f);

	//// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around


	//// Get a handle for our "MVP" uniform
	//// Only during the initialisation
	//GLuint MatrixID = glGetUniformLocation(shader, "MVP");

	//// Send our transformation to the currently bound shader, in the "MVP" uniform
	//// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);




	/* GL_PROJECTION is used for projecting geomtry to 2d screen f.ex. aspect ratio, center of projection
	 GL_MODELVIEW is used for position of model relative to camera, can be used to move camera, or objects (single objects as well)
	 glPushMatrix() and glPopMatrix is basically a way to backup a matrix on the stack and to restore it later*/

	//glEnable(GL_TEXTURE_2D);// Enable 2D texturing
	//glShadeModel(GL_SMOOTH);// Choose a smooth shading model
	//glClearColor(0.0, 0.0, 0.0, 0.0);// Set the clear color to black
	//glMatrixMode(GL_PROJECTION); // from now on work with the projection stack
	////gluLookAt(-100, -1000, -1000);
	//glLoadIdentity(); // make sure that the projection stack doesn't already have anything on it
	//glOrtho(0, screenWidth, 0, screenHeight, -1000, 2000); // where the geomtry is cut off /clipped in x/y/z axis ( 0 1920 0 1080 -1000 2000)
	//glMatrixMode(GL_MODELVIEW); //  from now on all changes applied to MODELVIEW 
	//glLoadIdentity(); // make sure that the modelview stack doesn't already have anything on it	
	//glTranslatef(-350.0f, 700, 0.0f); // the rotation will move the picture out of focus, adjust
	//glRotatef(77.264f, 1.0f, 0.0f, 0.0f); // rotation around x axis, for isometric view this should be 35.264
	//glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // rotation around z axis, for isometric view this should be -45
}
GameClass::~GameClass()
{
}

void GameClass::Draw()
{	
	//InitCamera();
	terrain.Draw();
}

void GameClass::UpdateWorld()
{
}