#include "glad/glad.h"
#include "objects/shader.hpp"
#include "objects/camera.hpp"
#include "objects/mesh.hpp"
#include "objects/skybox.hpp"
#include "world.hpp"
#include "chunk.hpp"
#include "noise.hpp"

#include "stb_image.h"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
bool	skybox_active = true;

GLFWwindow*	glfw_init_window(void);
void	draw(GLFWwindow* window, obj::shader &ourShader, std::vector<obj::Chunk *> &chunks, obj::Skybox &skybox, math::mat4 &model);

void	framebuffer_size_callback(GLFWwindow* window, int width, int height);
void	mouse_callback(GLFWwindow* window, double xpos, double ypos);
void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void	processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
obj::camera camera(math::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
std::mutex map_mutex;
std::mutex vector_mutex;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::map<std::pair<int, int>, obj::Chunk *> chunksMap;

void update_chunk(int renderDistance, std::vector<obj::Chunk *> *toDrawChunks)
{
	math::vec2 lastPos = math::vec2(312312, -312321);
	std::vector<obj::Chunk *> tmpChunks;
	while (true)
	{
		if (lastPos == math::vec2(camera.Position.x / 16, -camera.Position.z / 16))
		{
			continue;
		}
		for (int i = camera.Position.x / 16 - renderDistance; i <= camera.Position.x / 16 + renderDistance; i++)
		{
			for (int j = -camera.Position.z / 16 - renderDistance; j <= -camera.Position.z / 16+ renderDistance; j++)
			{
				map_mutex.lock();
				if (chunksMap.find(std::make_pair(i, j)) == chunksMap.end())
				{
					obj::Chunk *chunk = new obj::Chunk(i, j);
					chunksMap[std::make_pair(i, j)] = chunk;
					tmpChunks.push_back(chunk);
					map_mutex.unlock();
					continue;
				}
				map_mutex.unlock();
				tmpChunks.push_back(chunksMap[std::make_pair(i, j)]);
			}
		}
		vector_mutex.lock();
		*toDrawChunks = tmpChunks;
		vector_mutex.unlock();
		lastPos = math::vec2(camera.Position.x / 16, -camera.Position.z / 16);
	}
}

int main()
{
	GLFWwindow* window = glfw_init_window();
	if (window == nullptr)
		return -1;


	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader dirtUpgradezprogram
	obj::shader ourShader("srcs/objects/textures/texture.vs", "srcs/objects/textures/texture.fs", nullptr);

	std::vector<obj::Chunk *> chunks;
	int renderDistance = 10;
	obj::Chunk *chunk = new obj::Chunk(312312,321312);
	chunk->add_texture("dirt.png", "srcs/objects/textures/minecraft/dirt");
	chunk->add_texture("grassSide2.png", "srcs/objects/textures/minecraft");
	chunk->add_texture("grassTop.png", "srcs/objects/textures/minecraft");


	// SKY BOX
	obj::Skybox skybox;
	skybox.setup();
	skybox.load();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	std::vector<obj::Chunk *> toDrawChunks;	
	// render loop
	math::mat4 model = math::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = math::rotate(model, math::radians(90.0f), math::vec3(1.0f, 0.0f, 0.0f));
	model = model.translate(-10.0f, -15.0f, -5.0f);
	std::thread thread(update_chunk, renderDistance, &toDrawChunks);
	while (!glfwWindowShouldClose(window))
	{
		draw(	window,
				ourShader,
				toDrawChunks,
				skybox,
				model);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

GLFWwindow*	glfw_init_window(void)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scop", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

void	draw(GLFWwindow* window, obj::shader &ourShader, std::vector<obj::Chunk *> &toDrawChunks, obj::Skybox &skybox, math::mat4 &model)
{
	// per-frame time logic
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	int renderDistance = 10;
	// input
	processInput(window);

	// clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ourShader.use();
	// pass projection matrix to shader (note that in this case it could change every frame)
	math::mat4 projection = math::perspective(math::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	// camera/view transformation
	math::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);

		ourShader.setMat4("model", model);
		std::vector<std::pair<int, int>> toDelete;
		vector_mutex.lock();
		for (auto chunk: toDrawChunks)
		{
			int px = chunk->posX;
			int py = chunk->posY; 
			if (px < camera.Position.x / 16 - renderDistance || px > camera.Position.x / 16 + renderDistance || py < -camera.Position.z / 16 - renderDistance || py > -camera.Position.z / 16 + renderDistance)
			{
				toDelete.push_back(std::make_pair(px, py));
				continue;
			}
			if (chunk->isVAO == false)
			{
				chunk->setupMesh();
			}
			chunk->draw(ourShader);
			
		}
		
		map_mutex.lock();

		map_mutex.unlock();
		vector_mutex.unlock();
		if (skybox_active)
		{
			skybox.draw(view, projection);
		}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();
}


bool b_pressed = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::FORWARD, deltaTime * 5.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::BACKWARD, deltaTime * 5.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::LEFT, deltaTime * 5.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::RIGHT, deltaTime * 5.0f);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		b_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && b_pressed)
	{
		b_pressed = false;
		skybox_active = !skybox_active;
		std::cout << "skybox_active: " << skybox_active << std::endl;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	(void)window;
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset*0.1, yoffset* 0.1, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
