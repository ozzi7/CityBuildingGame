#include "stdafx.h"
#include "game.h"

GameEventHandler* unitEventHandler;
SoundEventHandler* soundEventHandler;

Game::Game(){};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler) {
	window = aWindow;
	inputHandler = aInputHandler;

	grid = new Grid(MAP_HEIGHT, MAP_WIDTH);
	unitEventHandler = new GameEventHandler(grid);
	soundEventHandler = new SoundEventHandler(255);

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
	std::thread threadSoundLoop(&Game::soundLoop, this);
	SetThreadPriority(&threadGameLoop, THREAD_PRIORITY_TIME_CRITICAL);
	
	renderLoop();
	threadGameLoop.join();
	threadSoundLoop.join();
}

void Game::renderLoop()
{
	renderer = new Renderer(*camera);
	shadow = new Shadow();
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 lightSpaceMatrix;
	
	camera->DirectionalLight.Color = { 1.0f, 1.0f, 1.0f };
	camera->DirectionalLight.PositionOffset = glm::vec3{ -1.0f, -2.0f, 1.0f };
	camera->UpdateLightDirection();

	grid->terrain->InitOpenGL(renderer->terrain_shader);
	shadow->InitShadowMap();

	while (!glfwWindowShouldClose(window))
	{
		// Shadow pass
		shadow->BindShadowMap();
		projection = camera->GetLightProjectionMatrix();
		view = camera->GetLightViewMatrix();
		lightSpaceMatrix = projection * view;

		renderer->SetShadowMatrices(projection, view, lightSpaceMatrix);
		renderer->CalculateShadow(renderBuffers->GetConsumerBuffer());

		// Render pass
		shadow->UnbindShadowMap();
		projection = camera->GetProjectionMatrix();
		view = camera->GetViewMatrix();

		renderer->SetMatrices(projection, view, lightSpaceMatrix, shadow->DepthMap);
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

		for (int i = 0; i < grid->gridUnits.size(); i++) {
			for (int j = 0; j < grid->gridUnits[i].size(); j++) {
				for (std::list<BoneAnimated*>::iterator it = grid->gridUnits[i][j]->movingObjects.begin();
					it != grid->gridUnits[i][j]->movingObjects.end(); ++it) {
					(*it)->UpdatePosition(grid);
					(*it)->GameStep();
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


		/*Handle all object moving, deleting, creating, no locks needed because no other thread is currently doing anything..*/
		while (unitEventHandler->ProcessEvent());
		
		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = (next_game_tick + std::chrono::microseconds(SKIP_TICKS));
		loops++;
	}
}
void Game::soundLoop()
{
	soundEventHandler->LoadFiles();
	
	while (!glfwWindowShouldClose(window))
	{
		while (soundEventHandler->ProcessEvent());
		std::this_thread::sleep_for(std::chrono::milliseconds(long((1.0/60.0)*1000))); // blocking queue cant be terminated..
	}
}
