#include "glad/glad.h"
#include "objects/Shader.hpp"
#include "objects/Camera.hpp"
#include "objects/Skybox.hpp"
#include "objects/TextureLoader.hpp"
#include "objects/TextureArray.hpp"
#include "Chunk.hpp"
#include "Noise.hpp"

#include "stb_image.h"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <map>
#include <filesystem>

bool	skybox_active = true;

GLFWwindow*	glfw_init_window(void);
void	draw(GLFWwindow* window, obj::Shader &ourShader, obj::Skybox &skybox);

void	framebuffer_size_callback(GLFWwindow* window, int width, int height);
void	mouse_callback(GLFWwindow* window, double xpos, double ypos);
void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void	processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
obj::Camera camera(math::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
std::mutex chunk_mutex;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const unsigned int renderDistance = 5;
std::map<std::pair<int, int>, obj::Chunk *>	chunksMap;


void	update_chunk(GLFWwindow* window, math::vec2 lastPos)
{
	std::vector<obj::Chunk *>					tmpDrawChunks;

	if (lastPos == math::vec2(camera.Position.x / 16, -camera.Position.y / 16))
		return;
	chunk_mutex.lock();
	std::map<std::pair<int, int>, obj::Chunk *>::iterator chunk;
	for (chunk = chunksMap.begin(); chunk != chunksMap.end(); chunk++)
	{
		if (std::abs(chunk->first.first - camera.Position.x / 16) > renderDistance ||
			std::abs(chunk->first.second - camera.Position.z / 16) > renderDistance)
		{
			// std::cout << "Chunk deleted at: " << chunk->first.first << " " << chunk->first.second << std::endl;
			delete chunk->second;
			// std::cout << "deleted" << std::endl;
			chunksMap.erase(chunk);
			// std::cout << "erased" << std::endl;
			chunk = chunksMap.begin();
		}
	}
	chunk_mutex.unlock();
	for (int i = 0; i < renderDistance; i++)
	{
		for (int j = 0; j < renderDistance; j++)
		{
			if (i + j > renderDistance)
				continue;
			int x = camera.Position.x / 16 + i;
			int y = camera.Position.z / 16 + j;
			chunk_mutex.lock();
			std::map<std::pair<int, int>, obj::Chunk *>::iterator found = chunksMap.find(std::make_pair(x, y));
			if (found == chunksMap.end())
			{
				obj::Chunk *chunk = new obj::Chunk(x, y);
				chunksMap[std::make_pair(x, y)] = chunk;
				// std::cout << "Chunk created at: " << x << " " << y << std::endl;
			}
			chunk_mutex.unlock();
		}
	}
	lastPos = math::vec2(camera.Position.x / 16, -camera.Position.y / 16);
}

int main()
{
	GLFWwindow* window = glfw_init_window();
	if (window == nullptr)
		return -1;

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader dirtUpgradezprogram
	obj::Shader ourShader("srcs/shaders/vertex_shader.vs", "srcs/shaders/fragment_shader.fs", nullptr);

	obj::TextureArray textureArray;
	obj::TextureLoader textureLoader;

	bool res = textureLoader.LoadTextureArray(
	{
		std::filesystem::path("textures/dirt.jpg"),
		std::filesystem::path("textures/grass.jpg"),
		std::filesystem::path("textures/stone.jpg"),
		std::filesystem::path("textures/bedrock.jpg"),
		std::filesystem::path("textures/sand.jpg"),
		std::filesystem::path("textures/oak_wood_side.jpg"),
		std::filesystem::path("textures/leaves_2.jpg"),
		std::filesystem::path("textures/water.jpg"),
		std::filesystem::path("textures/snow.jpg"),
		std::filesystem::path("textures/iron_ore.jpg"),
		std::filesystem::path("textures/gold_ore.jpg"),
		std::filesystem::path("textures/diamond_ore.jpg"),
		std::filesystem::path("textures/UNKNOWN.jpg"),
	});
	if (res)
		textureArray = textureLoader.GetTextureArray();
	else
	{
		std::cout << "Failed to load texture array" << std::endl;
		return -1;
	}
	

	// SKY BOX
	obj::Skybox skybox;
	skybox.setup();
	skybox.load();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// render loop
	// std::thread	thread(update_chunk, window);
	math::vec2 lastPos = math::vec2(1, 1);
	while (!glfwWindowShouldClose(window))
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.id);
		update_chunk(window, lastPos);
		draw(window, ourShader, skybox);
	}
	// thread.detach();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void	draw(GLFWwindow* window, obj::Shader &ourShader, obj::Skybox &skybox)
{
	// per-frame time logic
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
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

	ourShader.setInt("TextureArraySize", 6);

	chunk_mutex.lock();
	for (auto chunk: chunksMap)
	{
		if (chunk.second->isVAO == false)
		{
			chunk.second->setupMesh();
		}
		chunk.second->draw(ourShader);
	}
	chunk_mutex.unlock();

	if (skybox_active)
		skybox.draw(view, projection);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();
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

bool b_pressed = false;

float speed = 5.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::FORWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::BACKWARD, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::LEFT, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::RIGHT, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::UP, deltaTime * speed);
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::DOWN, deltaTime * speed);

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

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 10.0f;
	else
		speed = 5.0f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speed = 2.5f;
	else
		speed = 5.0f;
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
