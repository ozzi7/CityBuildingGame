#include "stdafx.h"
#include "game.h"

EventHandler* unitEventHandler; // ?? save in every object?! same with grid?

Game::Game(){};

Game::Game(GLFWwindow* aWindow, InputHandler* aInputHandler) {
	window = aWindow;
	inputHandler = aInputHandler;
	unitEventHandler = new EventHandler();

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

	float variableShadows = 0.05f;
	float offsetX = -40.0f;
	float offsetY = -40.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Shadow pass
		shadow->BindShadowMap();

		//camera->Scroll(RIGHT, 20);
		//camera->Scroll(UP, 20);
		projection = camera->GetProjectionMatrix(); // TODO: change this to capture larger window

		float projectionIncrease = 1.0f; // TODO: test, projection also depends on view, which right now is moving.. 
		projection = glm::ortho(-SCREEN_RATIO * camera->ZoomLevel*projectionIncrease, SCREEN_RATIO * camera->ZoomLevel*projectionIncrease,
			-1.0f * camera->ZoomLevel *projectionIncrease, 1.0f * camera->ZoomLevel*projectionIncrease, 0.0f, 200.0f);
		//projection = glm::ortho<float>(-10, 10, -10, 10, -10, 20);//camera->GetProjectionMatrix(); // ratio 1?
		//projection = glm::ortho(-20.0f, 40.0f, -40.0f, 20.0f, -5.0f, 200.0f);
		view = camera->GetViewMatrix();

		offsetX += variableShadows;
		offsetY += variableShadows;

		renderer->directionalLight.Color = glm::vec3(1.0f,1.0f,1.0f);
		if (offsetX > 120.0f) {
			offsetX = -60.0f;
			offsetY = -60.0f;
		}
		glm::mat4 view = glm::lookAt(camera->Position+ glm::vec3(-60.0+ offsetX,-60.0+ offsetY,0.0),
			camera->Position + camera->lookat, glm::vec3(0.0, 0.0, 1.0));
		lightSpaceMatrix = projection * view;

		renderer->ShadowPass = true;
		renderer->SetMatrices(projection, view, lightSpaceMatrix);
		renderer->Render(renderBuffers->GetConsumerBuffer());
		//camera->Scroll(DOWN, 20);
		//camera->Scroll(LEFT, 20);
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

		for (int i = 0; i < grid->gridUnits.size(); i++) {
			for (int j = 0; j < grid->gridUnits[i].size(); j++) {
				int idx = 0;
				for (std::list<BoneAnimated*>::iterator it = grid->gridUnits[i][j]->movingObjects.begin();
					it != grid->gridUnits[i][j]->movingObjects.end(); ++it) {
					(*it)->UpdatePosition(grid);
					(*it)->unitIdx = idx;
					idx++;
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
		Event * e = unitEventHandler->GetEvent();
		BoneAnimated * toMove = NULL;
		while (e != NULL) {
			/* removes element by index, could use pointers instead but... */
			int count = 0;
			for (auto it = grid->gridUnits[e->fromY][e->fromX]->movingObjects.begin(); it !=
				grid->gridUnits[e->fromY][e->fromX]->movingObjects.end(); ++it) {
				if (count == e->index) {
					toMove = (*it);
					grid->gridUnits[e->fromY][e->fromX]->movingObjects.erase(it);
					break;
				}
				count++;
			}
	
			grid->gridUnits[e->toY][e->toX]->movingObjects.push_back(toMove); // could use event type here
			e = unitEventHandler->GetEvent(); 
		}
		
		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(next_game_tick - std::chrono::high_resolution_clock::now()));
		next_game_tick = (next_game_tick + std::chrono::microseconds(SKIP_TICKS));
		loops++;
	}
}
