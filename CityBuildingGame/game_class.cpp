#include "game_class.h"

GameClass::GameClass() {}
GameClass::~GameClass() {}
void GameClass::Init(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight)
{
	screenWidth = aScreenWidth;
	screenHeight = aScreenHeight;

	terrain.Initialize(aMapWidth, aMapHeight);
}
void GameClass::StartGame()
{
	camera = Camera(glm::vec3(20.0f, 0.0f, 50.0f), window);
	
	glEnable(GL_DEPTH_TEST);

	// Spawning a tree,
	Tree tree(glm::vec3(10.0f, 10.0f, 5.0f));
	trees.push_back(tree);
	std::string texture_path = exe_path + "\\tree2_3ds\\Tree2.3ds";
	std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
	treeModel = Model(texture_path.c_str());

	std::thread threadGameLoop(&GameClass::GameLoop, this);
	std::thread threadRenderLoop(&GameClass::RenderLoop, this);
	threadGameLoop.join();
	threadRenderLoop.join();
}
void GameClass::RenderLoop()
{	
	// input
	// -----
	ProcessInput(window);

	camera.lock_cursor_to_window();
	camera.mouse_scroll();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderTree.use();

	glm::mat4 projection = glm::ortho(-SCREEN_RATIO * camera.Zoom, SCREEN_RATIO * camera.Zoom, -1 * camera.Zoom, 1 * camera.Zoom, 1.0f, 1000.0f);
	shaderTree.setMat4("projection", projection);

	glm::mat4 view = camera.GetViewMatrix();
	shaderTree.setMat4("view", view);

	for (int i = 0; trees.size(); i++) {
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, trees[i].Position);
		
		shaderTree.setMat4("model", model2);

		treeModel.Draw(shaderTree);
	}


	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();
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
			terrain.Update();

			next_game_tick += SKIP_TICKS;
			loops++;
		}
	}
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void GameClass::ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.keyboard_scroll(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.keyboard_scroll(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.keyboard_scroll(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.keyboard_scroll(RIGHT, deltaTime);
}
