#include "game_class.h"

GameClass::GameClass(int aMapWidth, int aMapHeight, float aScreenRatio, string aExePath, Camera & aCamera, GLFWwindow & aWindow) {
	screenRatio = aScreenRatio;
	exe_path = aExePath;
	camera = &aCamera;
	window = & aWindow;
	terrain = &Terrain();

	terrain->Initialize(aMapWidth, aMapHeight);
}
GameClass::~GameClass() {}

void GameClass::StartGame()
{	

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetWindowFocusCallback(window, window_focus_callback);

	//camera->lock_cursor_to_window();
	//camera->mouse_scroll();

	glEnable(GL_DEPTH_TEST);

	// Spawning a tree,
	Tree tree(glm::vec3(10.0f, 10.0f, 5.0f));
	trees.push_back(tree);
	std::string texture_path = exe_path + "\\tree2_3ds\\Tree2.3ds";
	std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
	treeModel = Model(texture_path, false);
	terrainModel = Model(texture_path, false);

	shaderTree = Shader("vertex_shader.vert", "fragment_shader.frag");
	shaderTerrain = Shader("basic_lighting.vert", "basic_lighting.frag");

	glfwMakeContextCurrent(NULL);

	std::thread threadGameLoop(&GameClass::GameLoop, this);
	std::thread threadRenderLoop(&GameClass::RenderLoop, this);
	while (!glfwWindowShouldClose(window)){}
	threadGameLoop.join();
	threadRenderLoop.join();
}
void GameClass::RenderLoop()
{	
	glfwMakeContextCurrent(window);

	shaderTree.use();
	glm::mat4 projection = glm::ortho(-screenRatio * camera->Zoom, screenRatio * camera->Zoom, -camera->Zoom, camera->Zoom, 1.0f, 1000.0f);
	shaderTree.setMat4("projection", projection);

	glm::mat4 view = camera->GetViewMatrix();
	shaderTree.setMat4("view", view);

	while (true)
	{	
		ProcessInput();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/*for (int i = 0; trees.size(); i++) {
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, trees[i].Position);

			shaderTree.setMat4("model", model2);

			treeModel.Draw(shaderTree);
		}*/

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GameClass::GameLoop()
{		
	const int TICKS_PER_SECOND = 60;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	while (true)
	{
		DWORD next_game_tick = GetTickCount();
		int loops = 0;
		while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {

			next_game_tick += SKIP_TICKS;
			loops++;
		}
	}
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void GameClass::ProcessInput()
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void GameClass::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	//glViewport(0, 0, width, height);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void GameClass::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->mouse_zoom((float)yoffset);
}
// glfw: whenever the window receives focus, camera gets locked
void GameClass::window_focus_callback(GLFWwindow *window, int focused)
{
	if (focused) {
		camera->lock_cursor_to_window();
	}
}