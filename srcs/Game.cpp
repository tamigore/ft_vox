#include "Game.hpp"

using namespace game;

Game::Game(): keys({{'b', false}}), world(), ourShader("srcs/objects/textures/texture.vs", "srcs/objects/textures/texture.fs", nullptr)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	projection = math::mat4(1.0f);
	view = math::mat4(1.0f);
	model = math::mat4(1.0f);
}

Game::~Game()
{

}

// void	Game::update_chunk(int renderDistance, std::vector<obj::Chunk *> *toDrawChunks)
// {
// 	math::vec2 lastPos = math::vec2(312312, -312321);
// 	std::vector<obj::Chunk *> tmpChunks;
// 	while (glfwWindowShouldClose(window) == 0)
// 	{
// 		if (lastPos == math::vec2(camera.Position.x / 16, -camera.Position.z / 16))
// 		{
// 			continue;
// 		}
// 		for (int i = camera.Position.x / 16 - renderDistance; i <= camera.Position.x / 16 + renderDistance; i++)
// 		{
// 			for (int j = -camera.Position.z / 16 - renderDistance; j <= -camera.Position.z / 16+ renderDistance; j++)
// 			{
// 				map_mutex.lock();
// 				if (world.chunksMap.find(std::make_pair(i, j)) == world.chunksMap.end())
// 				{
// 					obj::Chunk *chunk = new obj::Chunk(i, j);
// 					world.chunksMap[std::make_pair(i, j)] = chunk;
// 					tmpChunks.push_back(chunk);
// 					map_mutex.unlock();
// 					continue;
// 				}
// 				map_mutex.unlock();
// 				tmpChunks.push_back(world.chunksMap[std::make_pair(i, j)]);
// 			}
// 		}
// 		vector_mutex.lock();
// 		*toDrawChunks = tmpChunks;
// 		vector_mutex.unlock();
// 		lastPos = math::vec2(camera.Position.x / 16, -camera.Position.z / 16);
// 	}
// }

// void	Game::draw(GLFWwindow *window, std::vector<obj::Chunk *> &toDrawChunks)
// {
// 	// per-frame time logic
// 	float currentFrame = static_cast<float>(glfwGetTime());
// 	deltaTime = currentFrame - lastFrame;
// 	lastFrame = currentFrame;
// 	int renderDistance = 10;
// 	// input
// 	processInput(window);

// 	// clear color buffer
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
// 	ourShader.use();
// 	// pass projection matrix to shader (note that in this case it could change every frame)
// 	projection = math::perspective(math::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
// 	ourShader.setMat4("projection", projection);

// 	// camera/view transformation
// 	view = camera.GetViewMatrix();
// 	ourShader.setMat4("view", view);

// 	ourShader.setMat4("model", model);
// 	std::vector<std::pair<int, int>> toDelete;
// 	vector_mutex.lock();
// 	for (auto chunk: toDrawChunks)
// 	{
// 		int px = chunk->posX;
// 		int py = chunk->posY; 
// 		if (px < camera.Position.x / 16 - renderDistance || px > camera.Position.x / 16 + renderDistance || py < -camera.Position.z / 16 - renderDistance || py > -camera.Position.z / 16 + renderDistance)
// 		{
// 			toDelete.push_back(std::make_pair(px, py));
// 			continue;
// 		}
// 		if (chunk->isVAO == false)
// 		{
// 			chunk->setupMesh();
// 		}
// 		chunk->draw(ourShader);
// 	}
// 	vector_mutex.unlock();

// 	if (world.skybox_active)
// 		world.skybox.draw(view, projection);

// 	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// 	glfwSwapBuffers(window);
// 	glfwPollEvents();
// }

// void	Game::threads(std::vector<obj::Chunk *> *toDrawChunks)
// {
// 	std::thread t1(&Game::update_chunk, 10, toDrawChunks);
// 	t1.detach();
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// void	Game::processInput(GLFWwindow *window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);

// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 		camera.ProcessKeyboard(obj::FORWARD, deltaTime * 5.0f);
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 		camera.ProcessKeyboard(obj::BACKWARD, deltaTime * 5.0f);
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 		camera.ProcessKeyboard(obj::LEFT, deltaTime * 5.0f);
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 		camera.ProcessKeyboard(obj::RIGHT, deltaTime * 5.0f);

// 	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
// 	{
// 		keys['b'] = true;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && keys['b'])
// 	{
// 		keys['b'] = false;
// 		world.skybox_active = !world.skybox_active;
// 		std::cout << "skybox_active: " << world.skybox_active << std::endl;
// 	}
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
// 	glViewport(0, 0, width, height);
// }

// // glfw: whenever the mouse moves, this callback is called
// void	mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
// {
// 	float xpos = static_cast<float>(xposIn);
// 	float ypos = static_cast<float>(yposIn);

// 	if (firstMouse)
// 	{
// 		lastX = xpos;
// 		lastY = ypos;
// 		firstMouse = false;
// 	}

// 	float xoffset = xpos - lastX;
// 	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

// 	lastX = xpos;
// 	lastY = ypos;

// 	camera.ProcessMouseMovement(xoffset*0.1, yoffset* 0.1, true);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// void	scroll_callback(GLFWwindow *window,  double xoffset, double yoffset)
// {
// 	(void)xoffset;
// 	camera.ProcessMouseScroll(static_cast<float>(yoffset));
// }


// GLFWwindow*	Game::glfw_init_window(void)
// {
// 	// glfw: initialize and configure
// 	glfwInit();
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
// 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

// 	// glfw window creation
// 	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scop", NULL, NULL);
// 	if (window == NULL)
// 	{
// 		std::cout << "Failed to create GLFW window" << std::endl;
// 		glfwTerminate();
// 		return nullptr;
// 	}
// 	glfwMakeContextCurrent(window);
// 	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
// 	glfwSetCursorPosCallback(window, mouse_callback);
// 	glfwSetScrollCallback(window, scroll_callback);

// 	// tell GLFW to capture our mouse
// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// 	// glad: load all OpenGL function pointers
// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
// 	{
// 		std::cout << "Failed to initialize GLAD" << std::endl;
// 		return nullptr;
// 	}
// 	return window;
// }
