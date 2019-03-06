#include "stdafx.h"
#include "game.h"

Game::Game(){};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler) {
	window = aWindow;
	inputHandler = aInputHandler;

	grid = new Grid(MAP_WIDTH, MAP_HEIGHT);
	renderBuffers = new TripleBuffer<RenderBuffer>();

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
	// delete grid;
}

void Game::StartGame()
{
	std::thread threadGameLoop(&Game::gameLoop, this);
	
	renderLoop();
	threadGameLoop.join();
}

void Game::renderLoop()
{
	renderer = new Renderer();
	shadow = new Shadow();
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 lightSpaceMatrix;
	
	//lightSource = new Camera(glm::vec3(-50.0f, -50.0f, 50.0f), window);
	grid->terrain->InitOpenGL(renderer->terrain_shader);
	shadow->InitShadowMap();

	while (!glfwWindowShouldClose(window))
	{
		// Shadow pass
		shadow->BindShadowMap();

		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 200.0f);
		view = camera->GetViewMatrix();
		lightSpaceMatrix = projection * view;
		
		renderer->ShadowPass = true;
		renderer->SetMatrices(projection, view, lightSpaceMatrix);
		renderer->Render(renderBuffers->GetConsumerBuffer());

		//glfwSwapBuffers(window);


		// Render pass
		shadow->UnbindShadowMap();

		projection = camera->GetProjectionMatrix();
		view = camera->GetViewMatrix();

		renderer->ShadowPass = false;
		renderer->SetMatrices(projection, view, lightSpaceMatrix);
		renderer->Render(renderBuffers->GetConsumerBuffer());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	delete renderer;
}

void Game::gameLoop()
{
	const int TICKS_PER_SECOND = 100;
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
				}
			}
		}

		grid->terrain->SetRenderWindow(camera->GridTopLeftVisible(),camera->GridTopRightVisible(), camera->GridBottomLeftVisible(),
			camera->GridBottomRightVisible());
		grid->UpdateVisibleList(camera->GridTopLeftVisible(), camera->GridTopRightVisible(), camera->GridBottomLeftVisible(),
			camera->GridBottomRightVisible());

		/* Extract data for the renderer*/
		RenderBuffer* producerBuffer = renderBuffers->GetProducerBuffer();
		for (int i = 0; i < grid->nofVisibleUnits; i++) {
			for (std::list<GameObject*>::iterator it = grid->visibleUnits[i]->objects.begin();
				it != grid->visibleUnits[i]->objects.end(); ++it) {
				(*it)->Accept(*producerBuffer);
			}
			for (std::list<BoneAnimated*>::iterator it = grid->visibleUnits[i]->movingObjects.begin();
				it != grid->visibleUnits[i]->movingObjects.end(); ++it) {
				(*it)->Accept(*producerBuffer);
			}
		}
		grid->terrain->Accept(*producerBuffer); // TODO
		renderBuffers->ExchangeProducerBuffer();

		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = (next_game_tick + std::chrono::microseconds(SKIP_TICKS));
		loops++;
	}
}
