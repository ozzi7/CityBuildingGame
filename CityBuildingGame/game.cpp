#include "game.h"

Game::Game(){};

Game::Game(int aMapWidth, int aMapHeight, float aScreenRatio, string aExePath, Camera & aCamera, GLFWwindow* aWindow) {
	screenRatio = aScreenRatio;
	exe_path = aExePath;
	camera = &aCamera;
	window = aWindow;

	mapHeight = aMapHeight;
	mapWidth = aMapWidth;
	grid = new Grid(aMapHeight, aMapHeight);

	camera->grid = grid;
}
Game::~Game()
{
	//delete grid;
	//delete renderer;
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

		glm::mat4 projection = glm::ortho(-screenRatio * camera->Zoom, screenRatio * camera->Zoom, -1.0f * camera->Zoom, 1 * camera->Zoom, -1000.0f, 1000.0f);
		glm::mat4 view = camera->GetViewMatrix();

		renderer->SetMatrices(projection, view);
		grid->terrain->Accept(*renderer);

		grid->visibleUnitsMutex.lock();
		grid->visibleUnitsRendering = grid->visibleUnitsToRender;
		vector<Unit*> *visibleUnitsTemp;
		if (grid->visibleUnitsToRender == 0) {
			visibleUnitsTemp = grid->visibleUnits0;
		}
		else if (grid->visibleUnitsToRender == 1) {
			visibleUnitsTemp = grid->visibleUnits1;
		}
		else if (grid->visibleUnitsToRender == 2) {
			visibleUnitsTemp = grid->visibleUnits2;
		}
		int nofUnits = grid->visibleUnitsSizeToRender;
		grid->visibleUnitsMutex.unlock();

		for (int i = 0; i < nofUnits; i++) {
			for (list<Object*>::iterator it = (*visibleUnitsTemp)[i]->objects.begin(); it != (*visibleUnitsTemp)[i]->objects.end(); ++it) {
				(*it)->Accept(*renderer);
			}
		}
		renderer->RenderAll();
		glfwSwapBuffers(window);
	}
}
void Game::GameLoop()
{
	const int TICKS_PER_SECOND = 240;
	const int SKIP_TICKS = 1000000 / TICKS_PER_SECOND; // microseconds


	int loops = 0;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point next_game_tick(start + std::chrono::microseconds(SKIP_TICKS));
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
		ProcessInput();
		camera->mouse_scroll();

		grid->terrain->SetRenderWindow(camera->GetTopLeftVisible(),camera->GetTopRightVisible(), camera->GetBottomLeftVisible(),
			camera->GetBottomRightVisible());
		grid->UpdateVisibleList(camera->GetTopLeftVisible(), camera->GetTopRightVisible(), camera->GetBottomLeftVisible(),
			camera->GetBottomRightVisible());

		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = (next_game_tick + std::chrono::microseconds(SKIP_TICKS));
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

void Game::ProcessMouseclick(int button, int action, int mods) {
	
	if (!action == GLFW_PRESS) {return;}

	glm::vec2 cursor_position = camera->CursorPositionGrid();

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		try {
			if (cursor_position.x >= 0 && cursor_position.y >= 0 && grid->gridUnits.at((int)cursor_position.y).at((int)cursor_position.x)) {
				grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->objects.push_back(
					new Fir(glm::vec3(cursor_position.x, cursor_position.y, grid->gridUnits[(int)cursor_position.y][(int)cursor_position.x]->averageHeight),
						glm::vec3(1.0f, 1.0f, 1.0f),
						1.0f));
			}
		}
		catch(const std::out_of_range & e) {
			std::cout << "Cannot insert outside of grid" << std::endl;
		}
		
	}
}