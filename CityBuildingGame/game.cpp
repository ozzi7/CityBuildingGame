#include "stdafx.h"
#include "game.h"

Game::Game(){};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler) {
	window = aWindow;
	inputHandler = aInputHandler;

	grid = new Grid(MAP_WIDTH, MAP_HEIGHT);
	camera = new Camera(glm::vec3(50.0f + MAP_HEIGHT * 0.5f, -50.0f + MAP_WIDTH * 0.5f, 50.0f), window);

	inputHandler->Camera = camera;
	inputHandler->Window = window;
	inputHandler->Grid = grid;
	inputHandler->WindowFocus(true);

	camera->Grid = grid;

	MapGenerator* mapGenerator = new MapGenerator(grid);
	mapGenerator->GenerateMap();
	delete mapGenerator;
}
Game::~Game()
{
	//delete grid;
	//delete renderer;
}

void Game::StartGame()
{
	//glfwMakeContextCurrent(NULL);

	std::thread threadGameLoop(&Game::gameLoop, this);
	
	renderLoop();
	threadGameLoop.join();
}

void Game::renderLoop()
{
	//glfwMakeContextCurrent(window);

	renderer = new Renderer();
	
	grid->terrain->InitOpenGL(renderer->terrain_shader);

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();

		renderer->SetMatrices(projection, view);
		renderer->OpenGLStart();

		/* Render terrain */
		//grid->terrain->Accept(*renderer);

		/* TODO: write a class which handles 3 buffers and mutex to exchange*/
		grid->visibleUnitsMutex.lock();
		grid->visibleUnitsRendering = grid->visibleUnitsToRender;
		std::vector<Unit*> *visibleUnitsTemp;
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

		/* Render dynamic objects */
		/*for (int i = 0; i < nofUnits; i++) {
			for (list<Object*>::iterator it = (*visibleUnitsTemp)[i]->objects.begin(); it != (*visibleUnitsTemp)[i]->objects.end(); ++it) {
				(*it)->Accept(*renderer);
			}
			for (list<BoneAnimated*>::iterator it = (*visibleUnitsTemp)[i]->movingObjects.begin(); it != (*visibleUnitsTemp)[i]->movingObjects.end(); ++it) {
				(*it)->Accept(*renderer);
			}
		}*/

		/* Render instanced objects */
		renderer->RenderInstancedObjects();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

void Game::gameLoop()
{
	const int TICKS_PER_SECOND = 240;
	const int SKIP_TICKS = 1000000 / TICKS_PER_SECOND; // microseconds


	int loops = 0;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point next_game_tick(start + std::chrono::microseconds(SKIP_TICKS));
	while (!glfwWindowShouldClose(window))
	{
		//glfwPollEvents();
		inputHandler->MouseScroll();

		/* TODO: temp */
		for (int i = 0; i < grid->gridUnits.size(); i++) {
			for (int j = 0; j < grid->gridUnits[i].size(); j++) {
				for (std::list<BoneAnimated*>::iterator it = grid->gridUnits[i][j]->movingObjects.begin();
					it != grid->gridUnits[i][j]->movingObjects.end(); ++it) {
					(*it)->UpdatePosition();
					//std::cout << (*it)->position[0] << " | " <<
					//	(*it)->position[1] << " | " << 
					//	(*it)->position[2] << endl;
				}
			}
		}

		grid->terrain->SetRenderWindow(camera->GridTopLeftVisible(),camera->GridTopRightVisible(), camera->GridBottomLeftVisible(),
			camera->GridBottomRightVisible());
		grid->UpdateVisibleList(camera->GridTopLeftVisible(), camera->GridTopRightVisible(), camera->GridBottomLeftVisible(),
			camera->GridBottomRightVisible());

		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = (next_game_tick + std::chrono::microseconds(SKIP_TICKS));
		loops++;
	}
}
