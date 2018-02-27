#include "game_class.h"

GameClass::GameClass(int aMapWidth, int aMapHeight, float aScreenRatio, string aExePath, Camera & aCamera, GLFWwindow* aWindow) {
	screenRatio = aScreenRatio;
	exe_path = aExePath;
	camera = &aCamera;
	window = aWindow;

	mapHeight = aMapHeight;
	mapWidth = aMapWidth;
	terrain = new Terrain();
}
GameClass::~GameClass() 
{
	delete terrain;
}

void GameClass::StartGame()
{	
	camera->lock_cursor_to_window();
	camera->mouse_scroll();

	// Spawning a tree,
	Tree tree(glm::vec3(10.0f, 10.0f, 5.0f));
	trees.push_back(tree);
	std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
	std::string texture_path = exe_path + "/tree2_3ds/Tree2.3ds";
	treeModel = Model(texture_path, false);
	
	shaderTree = new Shader("vertex_shader.vert", "fragment_shader.frag");
	shaderTerrain = new Shader("basic_lighting.vert", "basic_lighting.frag");

	terrain->Initialize(mapWidth, mapHeight);
	terrain->LoadTextures(*shaderTerrain, exe_path);
	terrain->GenerateBuffers();
	glfwMakeContextCurrent(NULL);

	std::thread threadRenderLoop(&GameClass::RenderLoop, this);
	GameLoop();
	threadRenderLoop.join();
}
void GameClass::RenderLoop()
{	
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);

	while (true)
	{	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render terrain...
		// light properties
		glm::vec3 lightColor;
		lightColor.x = 1.0f;//sin(glfwGetTime() * 2.0f);
		lightColor.y = 1.0f;// sin(glfwGetTime() * 0.7f);
		lightColor.z = 1.0f;// sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		shaderTerrain->setVec3("light.ambient", ambientColor);
		shaderTerrain->setVec3("light.diffuse", diffuseColor);
		//ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		shaderTerrain->setVec3("light.position", camera->Position);
		shaderTerrain->setVec3("viewPos", camera->Position);

		//glm::mat4 projection = glm::ortho(-1.77777f * camera.Zoom, 1.77777f * camera.Zoom, -1 * camera.Zoom, 1 * camera.Zoom, 1.0f, 1000.0f);
		glm::mat4 projection = glm::ortho(-1.77777f * camera->Zoom, 1.77777f * camera->Zoom, -1 * camera->Zoom, 1 * camera->Zoom, 1.0f, 1000.0f);
		shaderTerrain->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();
		shaderTerrain->setMat4("view", view);

		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		shaderTerrain->setMat4("model", model);

		terrain->Draw(*shaderTerrain);


		// render tree...

		//shaderTree.use();
		//glm::mat4 projection = glm::ortho(-screenRatio * camera->Zoom, screenRatio * camera->Zoom, -camera->Zoom, camera->Zoom, 1.0f, 1000.0f);
		//shaderTree.setMat4("projection", projection);

		//glm::mat4 view = camera->GetViewMatrix();
		//shaderTree.setMat4("view", view);

		/*for (int i = 0; trees.size(); i++) {
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, trees[i].Position);

			shaderTree.setMat4("model", model2);

			treeModel.Draw(shaderTree);
		}*/

		glfwSwapBuffers(window);
	}
}

void GameClass::GameLoop()
{		
	const int TICKS_PER_SECOND = 60;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	while(true)
	{

		DWORD next_game_tick = GetTickCount();
		int loops = 0;
		while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {

			glfwPollEvents();
			ProcessInput();
			terrain->SetRenderWindow(0, 100, 0, 110);

			next_game_tick += SKIP_TICKS;
			loops++;
		}
	}
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void GameClass::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera->keyboard_scroll(UP);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera->keyboard_scroll(DOWN);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera->keyboard_scroll(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera->keyboard_scroll(RIGHT);
}
