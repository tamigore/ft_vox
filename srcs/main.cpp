#include "glad/glad.h"
#include "objects/Shader.hpp"
#include "objects/Camera.hpp"
#include "objects/Skybox.hpp"
#include "objects/TextureLoader.hpp"
#include "objects/TextureArray.hpp"
#include "ChunkGenerator.hpp"
#include "Chunk.hpp"
#include "Noise.hpp"

#include "stb_image.h"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <map>
#include <filesystem>

#include <future>
#include <thread>

GLFWwindow*	glfw_init_window(void);
void		draw(GLFWwindow* window, obj::Shader &ourShader, obj::Skybox &skybox);

void		framebuffer_size_callback(GLFWwindow* window, int width, int height);
void		mouse_callback(GLFWwindow* window, double xpos, double ypos);
void		scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void		processInput(GLFWwindow *window);

bool	skybox_active = true;
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

const unsigned int renderDistance = 15;
std::map<std::pair<int, int>, obj::Chunk *>	chunksMap;
ChunkGenerator	generator = ChunkGenerator(42);

void	generate(int i, int j)
{
	int x = camera.Position.x / 16 + i - renderDistance;
	int y = camera.Position.z / 16 + j - renderDistance;
	chunk_mutex.lock();
	std::map<std::pair<int, int>, obj::Chunk *>::iterator found = chunksMap.find(std::make_pair(x, y));
	if (found == chunksMap.end())
	{
		obj::Chunk *chunk = generator.generateChunk(x, y);
		found = chunksMap.find(std::make_pair(x - 1, y));
		if (found != chunksMap.end())
		{
			chunk->setWest(found->second);
			found->second->setEast(chunk);
		}
		found = chunksMap.find(std::make_pair(x + 1, y));
		if (found != chunksMap.end())
		{
			chunk->setEast(found->second);
			found->second->setWest(chunk);
		}
		found = chunksMap.find(std::make_pair(x - 1, y));
		if (found != chunksMap.end())
		{
			chunk->setSouth(found->second);
			found->second->setNorth(chunk);
		}
		found = chunksMap.find(std::make_pair(x + 1, y));
		if (found != chunksMap.end())
		{
			chunk->setNorth(found->second);
			found->second->setSouth(chunk);
		}
		chunksMap[std::make_pair(x, y)] = chunk;
	}
	chunk_mutex.unlock();
}

void createFaces()
{
	chunk_mutex.lock();
	std::vector<std::thread>	createFaces;
	for (auto chunk: chunksMap)
	{
		if (chunk.second->isCreated == false)
			createFaces.push_back(std::thread(&obj::Chunk::generateFaces, chunk.second));
	}
	for(int i = 0; i < createFaces.size() ; i++)
	{
		createFaces.at(i).join();
	}
	chunk_mutex.unlock();
}

// void createFaces(obj::Chunk *chunk)
// {
// 	if (chunk->isCreated == false)
// 		chunk->generateFaces();
// }

void	delete_chunk()
{
	std::map<std::pair<int, int>, obj::Chunk *>::iterator	chunk;
	std::vector<std::pair<int, int>>						set;
	chunk_mutex.lock();
	for (chunk = chunksMap.begin(); chunk != chunksMap.end(); chunk++)
	{
		if (std::abs(chunk->first.first - camera.Position.x / 16) > renderDistance ||
			std::abs(chunk->first.second - camera.Position.z / 16) > renderDistance)
		{
			set.push_back(chunk->first);
		}
	}
	for (auto key : set) { 
		delete chunksMap[key];
		chunksMap.erase(key); 
	} 
	chunk_mutex.unlock();
}

void	update_chunk(std::chrono::milliseconds timeBudget, std::vector<std::thread *> &generation, int &g_pos, std::vector<std::thread *> &create, int &c_pos, bool mooved)
{
	std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();
	if (mooved)
	{
		delete_chunk();
		for (int i = 0; i < renderDistance * 2; i++)
		{
			for (int j = 0; j < renderDistance * 2; j++)
				generation.push_back(new std::thread(generate, i, j));
		}
	}
	while (g_pos < generation.size())
	{
		if (std::chrono::high_resolution_clock::now() - start > timeBudget)
			break;
		generation.at(g_pos)->join();
		delete generation.at(g_pos);
		g_pos++;
	}
	if (g_pos == generation.size())
	{
		g_pos = 0;
		generation.clear();
	}
	// chunk_mutex.lock();
	// for (auto chunk: chunksMap)
	// {
	// 	if (chunk.second->isCreated == false)
	// 		create.push_back(new std::thread(createFaces, chunk.second));
	// }
	// while (c_pos < generation.size())
	// {
	// 	if (std::chrono::high_resolution_clock::now() - start > timeBudget)
	// 		return;
	// 	create.at(c_pos)->join();
	// 	delete create.at(c_pos);
	// }
	// chunk_mutex.unlock();
	createFaces();
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
	int lastX = 0;
	int lastY = 0;
	int g_pos = 0;
	int c_pos = 0;
	bool mooved = true;
	std::vector<std::thread *>	generation;
	std::vector<std::thread *>	create;
	while (!glfwWindowShouldClose(window))
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.id);
		if (lastX != int(camera.Position.x / 16) || lastY != int(camera.Position.y / 16))
		{
			mooved = true;
			lastX = camera.Position.x / 16;
			lastY = camera.Position.y / 16;
		}
		update_chunk(std::chrono::milliseconds(20), generation, g_pos, create, c_pos, mooved);
		mooved = false;
		draw(window, ourShader, skybox);
	}
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
		speed = 20.0f;
	else
		speed = 5.0f;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		speed = 2.5f;
	else
		speed = 5.0f;
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		speed = 1.0f;
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
