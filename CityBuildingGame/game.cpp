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
	delete renderer;
}

void Game::StartGame()
{
	glfwMakeContextCurrent(NULL);

	std::thread threadRenderLoop(&Game::RenderLoop, this);
	GameLoop();
	threadRenderLoop.join();
}
void Game::RenderLoop()
{
	glfwMakeContextCurrent(window);
	renderer = new Renderer(exe_path);
	
	grid->terrain->InitOpenGL(renderer->shader_terrain, exe_path);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 projection = glm::ortho(-screenRatio * camera->Zoom, screenRatio * camera->Zoom, -1 * camera->Zoom, 1 * camera->Zoom, 1.0f, 1000.0f);
		glm::mat4 view = camera->GetViewMatrix();

		renderer->SetMatrices(projection, view);
		grid->terrain->Accept(*renderer);

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
	const int TICKS_PER_SECOND = 120;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	int loops = 0;
	DWORD next_game_tick = GetTickCount64() + SKIP_TICKS;
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
