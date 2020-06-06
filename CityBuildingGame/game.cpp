#include "stdafx.h"
#include "game.h"

GameEventHandler* gameEventHandler;
SoundEventHandler* soundEventHandler;
LoggingEventHandler* loggingEventHandler;
Resources* resources;

Game::Game() {};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler)
{
	window = aWindow;
	inputHandler = aInputHandler;

	grid = new Grid(MAP_HEIGHT, MAP_WIDTH);
	loggingEventHandler = new LoggingEventHandler();
	gameEventHandler = new GameEventHandler(grid);
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
	                                               "Creating map..."));

	MapGenerator* mapGenerator = new MapGenerator(grid);
	mapGenerator->GenerateMap();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "Done creating map"));

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
	renderer = new Renderer(*camera);

	// Only needs to be done the first time, afterwards handled by zoom and scroll events of camera
	camera->CalculateVisibleGrid();
	camera->CalculateLightProjectionMatrix();

	grid->terrain->InitOpenGL();
	renderer->InitShadowMap();

	while (!glfwWindowShouldClose(window))
	{
		// Shadow pass
		renderer->BindShadowMap();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 lightSpaceMatrix = projection * view;
		renderer->SetShadowMatrices(lightSpaceMatrix);
		renderer->CalculateShadow(renderBuffers->GetConsumerBuffer());

		// Render pass
		renderer->UnbindShadowMap();
		projection = camera->GetLightProjectionMatrix();
		view = camera->GetLightViewMatrix();
		renderer->SetMatrices(projection, view, lightSpaceMatrix, renderer->ShadowDepthMap);
		renderer->Render(renderBuffers->GetConsumerBuffer());
		

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	delete renderer;
}

void Game::gameLoop()
{
	const int TICKS_PER_SECOND = 100; // TODO: change back
	const int SKIP_TICKS = 1000000 / TICKS_PER_SECOND; // microseconds
	unsigned long int loopCount = 0;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point next_game_tick(start + std::chrono::microseconds(SKIP_TICKS));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		inputHandler->MouseScroll();

		lightingCalculation(loopCount);
		
		for (std::list<Worker*>::iterator it = resources->workers.begin(); 
			it != resources->workers.end(); ++it)
		{
			(*it)->UpdatePosition(grid);
			(*it)->GameStep();
		}
		for (std::list<Lumberjack*>::iterator it = resources->lumberjacks.begin();
			it != resources->lumberjacks.end(); ++it)
		{
			(*it)->UpdatePosition(grid);
			(*it)->GameStep();
		}

		grid->terrain->SetRenderWindow(camera->GridTopLeftVisible(),
									   camera->GridTopRightVisible(),
									   camera->GridBottomLeftVisible(),
									   camera->GridBottomRightVisible());
		grid->UpdateVisibleList(camera->GridTopLeftVisible(),
								camera->GridTopRightVisible(),
								camera->GridBottomLeftVisible(),
		                        camera->GridBottomRightVisible());

		//*Handle all object moving, deleting, creating, no locks needed because no other thread is currently doing anything..*/
		while (gameEventHandler->ProcessEvent());
		
		// TODO: prevent slowdowns
		if(loopCount % 100 == 0)
			gameEventHandler->AssignWorkToIdleWorkers();

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

		loopCount++;

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

void Game::lightingCalculation(unsigned long loopCount)
{
	float lightXOffset;
	float lightYOffset;
	glm::vec3 directionLightColor;
	glm::vec3 ambientLightColor = {1.0f, 1.0f, 1.0f};
	
	const float directionalLightIntensity = pow(BRIGHTNESS * 0.01 + 0.5f, 3.0f) * 0.9f;
	const float ambientLightIntensity = pow(BRIGHTNESS * 0.01 + 0.5f, 3.0f) * 0.4f;
	const float loopPercentage = (float)(loopCount % 5000) / 50.0f;
	
	// Change directional light position
	if (loopPercentage < 50)
	{
		lightXOffset = lightXOffset = -2.0f + loopPercentage / 100.0f * 8.0f;
		lightYOffset = lightXOffset - 0.5f;
	}
	else
	{
		lightXOffset = lightXOffset = -2.0f + (100.0f - loopPercentage) / 100.0f * 8.0f;
		lightYOffset = lightXOffset - 0.5f;
	}

	// Change directional light color
	if (loopPercentage < 20)
		directionLightColor = {directionalLightIntensity, directionalLightIntensity * (loopPercentage + 20.0f) / 40.0f, directionalLightIntensity * (loopPercentage + 20.0f) / 40.0f};
	else if (loopPercentage > 80)
		directionLightColor = {directionalLightIntensity, directionalLightIntensity * (100.0f - loopPercentage + 20.0f) / 40.0f, directionalLightIntensity * (100.0f - loopPercentage + 20.0f) / 40.0f};
	else if (loopPercentage >= 50 && loopPercentage < 70)
		directionLightColor = {directionalLightIntensity, directionalLightIntensity * (loopPercentage - 50.0f + 20.0f) / 40.0f, directionalLightIntensity * (loopPercentage - 50.0f + 20.0f) / 40.0f};
	else if (loopPercentage < 50 && loopPercentage > 30)
		directionLightColor = {directionalLightIntensity, directionalLightIntensity * (50.0f - loopPercentage + 20.0f) / 40.0f, directionalLightIntensity * (50.0f - loopPercentage + 20.0f) / 40.0f};
	else
		directionLightColor = {directionalLightIntensity, directionalLightIntensity, directionalLightIntensity};

	glm::vec3 ambientLight = {ambientLightColor.x + directionLightColor.x, ambientLightColor.y + directionLightColor.y, ambientLightColor.z + directionLightColor.z};
	ambientLight *= 0.5f;
	ambientLight *= ambientLightIntensity;

	camera->SetDirectionalLightPositionOffset(glm::vec3{lightXOffset, lightYOffset, 1.0f});
	camera->SetDirectionalLightColor(directionLightColor);
	camera->AmbientLight = ambientLight;
}