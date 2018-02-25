#include "game_class.h"

GameClass::GameClass(float aScreenWidth, float aScreenHeight, int aMapWidth, int aMapHeight)
{
	screenWidth = aScreenWidth;
	screenHeight = aScreenHeight;

	terrain.Initialize(aMapWidth, aMapHeight);
}

GameClass::~GameClass()
{
}
void GameClass::StartGame()
{
	camera = Camera(glm::vec3(20.0f, 0.0f, 50.0f), window);

	glEnable(GL_DEPTH_TEST);

	ourShader = Shader("basic_lighting.vert", "basic_lighting.frag");
	shaderTree = Shader("vertex_shader.vert", "fragment_shader.frag");

	std::string texture_path;

	// tree test model
	texture_path = exe_path + "\\tree2_3ds\\Tree2.3ds";
	std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
	Model tree(texture_path.c_str());

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

	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(20.0f, 20.0f, 20.0f));
	shaderTree.setMat4("projection", projection);
	shaderTree.setMat4("view", view);
	shaderTree.setMat4("model", model2);

	tree.Draw(shaderTree);

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
