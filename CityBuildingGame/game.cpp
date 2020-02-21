#include "stdafx.h"
#include "game.h"

GameEventHandler* unitEventHandler;
SoundEventHandler* soundEventHandler;
LoggingEventHandler* loggingEventHandler;
Resources* resources;

Game::Game() {};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler)
{
	window = aWindow;
	inputHandler = aInputHandler;

	grid = new Grid(MAP_HEIGHT, MAP_WIDTH);
	loggingEventHandler = new LoggingEventHandler(LoggingLevel::WARNING, LoggingLevel::INFO);
	unitEventHandler = new GameEventHandler(grid);
	soundEventHandler = new SoundEventHandler(255);
	resources = new Resources();

	renderBuffers = new TripleBuffer<RenderBuffer>();

	camera = new Camera(glm::vec3(50.0f + MAP_HEIGHT * 0.5f, -50.0f + MAP_WIDTH * 0.5f, 50.0f), window);

	inputHandler->Camera = camera;
	inputHandler->Window = window;
	inputHandler->Grid = grid;
	inputHandler->WindowFocus(true);

	camera->Grid = grid;

	// TODO: this is a test, gettickcount is not cross platform..
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
	                                               "Generating map..."));

	MapGenerator* mapGenerator = new MapGenerator(grid);
	mapGenerator->GenerateMap();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "Done generating map"));

	delete mapGenerator;
}

Game::~Game()
{
	// delete grid;
}

void Game::StartGame()
{
	std::thread threadGameLoop(&Game::gameLoop, this);
	std::thread threadSoundLoop(&Game::soundLoop, this);
	std::thread threadLoggingLoop(&Game::loggingLoop, this);
	SetThreadPriority(&threadGameLoop, THREAD_PRIORITY_TIME_CRITICAL);

	renderLoop();
	threadGameLoop.join();
	threadSoundLoop.join();
	threadLoggingLoop.join();
}

void Game::renderLoop()
{
	// TODO: Wait for game loop to finish once instead of sleep
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));

	renderer = new Renderer(*camera);

	camera->SetDirectionalLightColor(glm::vec3{1.0f, 1.0f, 1.0f});
	camera->SetDirectionalLightPositionOffset(glm::vec3{-0.4f, -0.8f, 1.0f});

	grid->terrain->InitOpenGL(renderer->terrain_shader);
	renderer->InitShadowMap();

	while (!glfwWindowShouldClose(window))
	{
		// Shadow pass
		renderer->BindShadowMap();
		glm::mat4 projection = camera->GetLightProjectionMatrix();
		glm::mat4 view = camera->GetLightViewMatrix();
		glm::mat4 lightSpaceMatrix = projection * view;
		renderer->SetShadowMatrices(projection, view, lightSpaceMatrix);
		renderer->CalculateShadow(renderBuffers->GetConsumerBuffer());

		// Render pass
		renderer->UnbindShadowMap();
		projection = camera->GetProjectionMatrix();
		view = camera->GetViewMatrix();
		renderer->SetMatrices(projection, view, lightSpaceMatrix, renderer->ShadowDepthMap);
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

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point next_game_tick(start + std::chrono::microseconds(SKIP_TICKS));

	while (!glfwWindowShouldClose(window))
	{
		//glfwPollEvents();
		inputHandler->MouseScroll();

		for (int i = 0; i < grid->gridUnits.size(); i++)
		{
			for (int j = 0; j < grid->gridUnits[i].size(); j++)
			{
				for (std::list<BoneAnimated*>::iterator it = grid->gridUnits[i][j]->movingObjects.begin();
				     it != grid->gridUnits[i][j]->movingObjects.end(); ++it)
				{
					(*it)->UpdatePosition(grid);
					(*it)->GameStep();
				}
			}
		}

		camera->CalculateVisibleGrid();
		grid->terrain->SetRenderWindow(camera->GridTopLeftVisible(),
										camera->GridTopRightVisible(),
										camera->GridBottomLeftVisible(),
										camera->GridBottomRightVisible());
		grid->UpdateVisibleList(camera->GridTopLeftVisible(),
								camera->GridTopRightVisible(),
								camera->GridBottomLeftVisible(),
		                        camera->GridBottomRightVisible());

		/* Extract data for the renderer*/
		RenderBuffer* producerBuffer = renderBuffers->GetProducerBuffer();
		for (int i = 0; i < grid->nofVisibleUnits; i++)
		{
			for (std::list<GameObject*>::iterator it = grid->visibleUnits[i]->objects.begin();
			     it != grid->visibleUnits[i]->objects.end(); ++it)
			{
				(*it)->Accept(*producerBuffer);
			}
			for (std::list<BoneAnimated*>::iterator it = grid->visibleUnits[i]->movingObjects.begin();
			     it != grid->visibleUnits[i]->movingObjects.end(); ++it)
			{
				(*it)->Accept(*producerBuffer);
			}
		}
		grid->terrain->Accept(*producerBuffer); // TODO
		renderBuffers->ExchangeProducerBuffer();

		//*Handle all object moving, deleting, creating, no locks needed because no other thread is currently doing anything..*/
		while (unitEventHandler->ProcessEvent());

		std::this_thread::sleep_for(
			std::chrono::duration_cast<std::chrono::microseconds>(
				next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = next_game_tick + std::chrono::microseconds(SKIP_TICKS);
	}
}

void Game::soundLoop() const
{
	soundEventHandler->LoadFiles();

	while (!glfwWindowShouldClose(window))
	{
		while (soundEventHandler->ProcessEvent());
		std::this_thread::
			sleep_for(std::chrono::milliseconds(long(1.0 / 60.0 * 1000))); // blocking queue cant be terminated..
	}
}

void Game::loggingLoop() const
{
	while (!glfwWindowShouldClose(window))
	{
		while (loggingEventHandler->ProcessEvent());
		std::this_thread::
			sleep_for(std::chrono::milliseconds(long(1.0 / 60.0 * 1000))); // blocking queue cant be terminated..
	}
}
