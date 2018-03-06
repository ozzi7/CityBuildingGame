#include "game.h"

Game::Game(int aMapWidth, int aMapHeight, float aScreenRatio, string aExePath, Camera & aCamera, GLFWwindow* aWindow) {
	screenRatio = aScreenRatio;
	exe_path = aExePath;
	camera = &aCamera;
	window = aWindow;

	mapHeight = aMapHeight;
	mapWidth = aMapWidth;
	grid = new Grid(aMapHeight, aMapHeight);
}
Game::~Game()
{
	delete grid;
	delete shaderTree;
	delete shaderTerrain;
	delete renderer;
}

void Game::StartGame()
{
	renderer = new Renderer();
	//std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
	//std::string texture_path = exe_path + "/tree2_3ds/Tree2.3ds";

	//std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
	//texture_path = exe_path + "/fir/Fir.obj";
	//firTreeModel = Model(texture_path, false);

	glfwMakeContextCurrent(NULL);

	std::thread threadRenderLoop(&Game::RenderLoop, this);
	GameLoop();
	threadRenderLoop.join();
}
void Game::RenderLoop()
{
	glfwMakeContextCurrent(window);

	renderer->shader_tree = new Shader("vertex_shader.vert", "fragment_shader.frag");
	std::replace(exe_path.begin(), exe_path.end(), '\\', '/');
	std::string texture_path = exe_path + "/tree2_3ds/Tree2.3ds";

	renderer->model_tree = new Model(texture_path, false);
	//shaderTerrain = new Shader("vertex_shader.vert", "fragment_shader.frag");
	shaderTerrain = new Shader("basic_lighting.vert", "basic_lighting.frag");

	glEnable(GL_DEPTH_TEST);

	shaderTerrain->use();
	grid->terrain->LoadTextures(*shaderTerrain, exe_path);
	grid->terrain->GenerateBuffers();

	while (!glfwWindowShouldClose(window))
	{
		shaderTerrain->use();

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
		glm::mat4 projection = glm::ortho(-screenRatio * camera->Zoom, screenRatio * camera->Zoom, -1 * camera->Zoom, 1 * camera->Zoom, 1.0f, 1000.0f);
		shaderTerrain->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera->GetViewMatrix();
		shaderTerrain->setMat4("view", view);

		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		shaderTerrain->setMat4("model", model);

		grid->terrain->Draw(*shaderTerrain);

		renderer->SetMatrices(projection, view);
		// render all objects
		for (int i = 0; i < grid->gridunits.size()-250; i++) {
			for (int j = 0; j < grid->gridunits[i].size()-250; j++) {
				for (auto it = grid->gridunits[i][j]->objects.begin(); it !=
					grid->gridunits[i][j]->objects.end(); ++it) {
						(*it)->Accept(*renderer);
				}
			}
		}

		glfwSwapBuffers(window);
	}
}

void Game::GameLoop()
{
	grid->terrain->InitializeRenderData(100, 100); // this sets the maximum visible range..

	const int TICKS_PER_SECOND = 120;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	int loops = 0;
	DWORD next_game_tick = GetTickCount() + SKIP_TICKS;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ProcessInput();

		grid->terrain->SetRenderWindow(camera->GetTopLeftVisible(),camera->GetTopRightVisible(), camera->GetBottomLeftVisible(),
			camera->GetBottomRightVisible());
		
		camera->mouse_scroll();

		std::this_thread::sleep_for(std::chrono::milliseconds(next_game_tick - GetTickCount64()));
		next_game_tick += SKIP_TICKS;
		loops++;
	}
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Game::ProcessInput()
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
