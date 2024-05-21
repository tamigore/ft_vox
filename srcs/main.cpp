#include "glad/glad.h"
#include "objects/Shader.hpp"
#include "objects/Camera.hpp"
#include "objects/Skybox.hpp"
#include "objects/TextureLoader.hpp"
#include "objects/TextureArray.hpp"
#include "objects/Noise.hpp"
#include "objects/Chunk.hpp"
#include "ChunkGenerator.hpp"
#include "Profiler.hpp"
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
void		draw(GLFWwindow* window, obj::Shader &ourShader, obj::Skybox &skybox, std::vector<obj::Chunk *>	*stable_state);

void		framebuffer_size_callback(GLFWwindow* window, int width, int height);
void		mouse_callback(GLFWwindow* window, double xpos, double ypos);
void		scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void		processInput(GLFWwindow *window);

// settings
bool	skybox_active = true;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
obj::Camera camera(math::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::mutex camMutex;
int stableCamX = 0;
int stableCamY = 0;

unsigned int renderDistance = 2;
ChunkGenerator generator = ChunkGenerator(42);

// Thread
std::mutex	chunk_mutex;
std::mutex	stable_mutex;

std::mutex	toDeleteVao_mutex;
std::vector<unsigned int> toDeleteVAO;

void	generate(int x, int y, std::map<std::pair<int, int>, obj::Chunk *> *chunksMap)
{
	obj::Chunk *chunk = nullptr;

	chunk_mutex.lock();
	std::map<std::pair<int, int>, obj::Chunk *>::iterator found = chunksMap->find(std::make_pair(x, y));
	if (found == chunksMap->end())
	{
		chunk = generator.generateChunk(x, y);
		found = chunksMap->find(std::make_pair(x - 1, y));
		if (found != chunksMap->end())
		{
			chunk->setWest(found->second);
			found->second->setEast(chunk);
		}
		found = chunksMap->find(std::make_pair(x + 1, y));
		if (found != chunksMap->end())
		{
			chunk->setEast(found->second);
			found->second->setWest(chunk);
		}
		found = chunksMap->find(std::make_pair(x, y - 1));
		if (found != chunksMap->end())
		{
			chunk->setSouth(found->second);
			found->second->setNorth(chunk);
		}
		found = chunksMap->find(std::make_pair(x, y + 1));
		if (found != chunksMap->end())
		{
			chunk->setNorth(found->second);
			found->second->setSouth(chunk);
		}
		(*chunksMap)[std::make_pair(x, y)] = chunk;
	}
	chunk_mutex.unlock();
}

void	delete_chunk(std::map<std::pair<int, int>, obj::Chunk *>	*chunksMap, int camX, int camY)
{
	std::map<std::pair<int, int>, obj::Chunk *>::iterator	chunk;
	std::vector<std::pair<int, int>>						set;
	chunk_mutex.lock();
	for (chunk = chunksMap->begin(); chunk != chunksMap->end(); chunk++)
	{
		if ((chunk->first.first < (camX - (int)renderDistance)) ||
			(chunk->first.first > (camX + (int)renderDistance) )||
			(chunk->first.second < (camY - (int)renderDistance)) ||
			(chunk->first.second > (camY + (int)renderDistance)))
		{
			set.push_back(chunk->first);
		}
	}
	for (auto key : set)
	{ 
		obj::Chunk* chunk = chunksMap->at(key);
		if (chunk->isVAO)
		{
			toDeleteVao_mutex.lock();
			toDeleteVAO.push_back(chunksMap->at(key)->getVAO());
			toDeleteVAO.push_back(chunksMap->at(key)->getVBO());
			toDeleteVAO.push_back(chunksMap->at(key)->getEBO());
			toDeleteVao_mutex.unlock();
		}
		if (chunk->getWest())
			chunk->getWest()->setEast(nullptr);
		if (chunk->getEast())
			chunk->getEast()->setWest(nullptr);
		if (chunk->getNorth())
			chunk->getNorth()->setSouth(nullptr);
		if (chunk->getSouth())
			chunk->getSouth()->setNorth(nullptr);
		delete chunk;
		chunksMap->erase(key);
	} 
	chunk_mutex.unlock();
}

void	delete_all_chunk(std::map<std::pair<int, int>, obj::Chunk *>	*chunksMap)
{
	std::map<std::pair<int, int>, obj::Chunk *>::iterator	chunk;
	chunk_mutex.lock();
	for (chunk = chunksMap->begin(); chunk != chunksMap->end(); chunk++)
	{
		if (chunk->second->isVAO)
		{
			toDeleteVao_mutex.lock();
			toDeleteVAO.push_back(chunk->second->getVAO());
			toDeleteVAO.push_back(chunk->second->getVBO());
			toDeleteVAO.push_back(chunk->second->getEBO());
			toDeleteVao_mutex.unlock();
		}
		delete chunk->second;
	}
	chunksMap->clear();
	chunk_mutex.unlock();
}

void update_thread(std::vector<obj::Chunk *> **stable_state, GLFWwindow *window)
{
	std::map<std::pair<int, int>, obj::Chunk *>	chunksMap;
	std::vector<obj::Chunk *>					*tmp_state = new std::vector<obj::Chunk *>;

	std::vector<std::thread>			generation;
	std::vector<std::pair<int, int>>	buffer;
	int camX, camY;
	int g_pos = 0;
	int	last_x = 13213;
	int	last_y = 3213;

	while (!glfwWindowShouldClose(window))
	{
		camMutex.lock();
		camX = stableCamX / 16;
		camY = stableCamY / 16;
		camMutex.unlock();
		if (last_x == camX && last_y == camY)
			continue;
		last_x = camX;
		last_y = camY;
		for (int i = 0; i <= renderDistance * 2; i++)
		{
			for (int j = 0; j <= renderDistance * 2; j++)
			{
				int posX = camX + i - renderDistance;
				int posY = camY + j - renderDistance;

				chunk_mutex.lock();
				if (chunksMap.find(std::make_pair(posX, posY)) == chunksMap.end())
				{
					chunk_mutex.unlock();
					generation.push_back(std::thread(generate, posX, posY, &chunksMap));
					buffer.push_back(std::make_pair(posX, posY));
				}
				else
				{
					tmp_state->push_back(chunksMap[std::make_pair(posX, posY)]);
					chunk_mutex.unlock();
				}
			}
		}
		while (g_pos < generation.size())
		{
			generation.at(g_pos).join();
			tmp_state->push_back(chunksMap[buffer.at(g_pos)]);
			g_pos++;
		}
		if (g_pos == generation.size())
		{
			g_pos = 0;
			generation.clear();
			buffer.clear();
		}
		for (auto chunk : *tmp_state)
			chunk->generateFaces();
		stable_mutex.lock();
		delete *stable_state;
		*stable_state = tmp_state;
		stable_mutex.unlock();
		tmp_state = new std::vector<obj::Chunk *>();

		delete_chunk(&chunksMap, camX, camY);
	}
	delete_all_chunk(&chunksMap);
	if (tmp_state != nullptr)
		delete tmp_state;
}

void	deleteBuffers()
{
	toDeleteVao_mutex.lock();
	for (int i = 0; i < toDeleteVAO.size(); i += 3)
	{
		unsigned int vao = toDeleteVAO[i];
		unsigned int vbo = toDeleteVAO[i + 1];
		unsigned int ebo = toDeleteVAO[i + 2];
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
	toDeleteVAO.clear();
	toDeleteVao_mutex.unlock();
}

int main()
{
	GLFWwindow* window = glfw_init_window();
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	// Profiler::SetSaveOn();

	// build and compile our shader dirtUpgradezprogram
	obj::Shader ourShader("srcs/shaders/vertex_shader.vs", "srcs/shaders/fragment_shader.fs", nullptr);

	obj::TextureArray textureArray;
	obj::TextureLoader textureLoader;

	bool res = textureLoader.LoadTextureArray(
	{
		std::filesystem::path("textures/grass.png"),
		std::filesystem::path("textures/grass.png"),
		std::filesystem::path("textures/grass_side.png"),
		std::filesystem::path("textures/snow.png"),
		std::filesystem::path("textures/snow_side.png"),
		std::filesystem::path("textures/NewWater.png"),
		std::filesystem::path("textures/dirt.png"),
		std::filesystem::path("textures/stone.png"),
		std::filesystem::path("textures/bedrock.png"),
		std::filesystem::path("textures/gravel.png"),
		std::filesystem::path("textures/sand.png"),
	});
	if (res)
		textureArray = textureLoader.GetTextureArray();
	else
	{
		std::cout << "Failed to load texture array" << std::endl;
		glfwTerminate();
		return -1;
	}

	// SKY BOX
	obj::Skybox skybox;
	skybox.setup();
	skybox.load();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// thread loop
	std::vector<obj::Chunk *>	*stable_state = new std::vector<obj::Chunk *>;
	std::thread	secondaryThread(update_thread, &stable_state, window);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		camMutex.lock();
		stableCamX = camera.Position.x;
		stableCamY = camera.Position.z;
		camMutex.unlock();
		deleteBuffers();
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.id);
		draw(window, ourShader, skybox, stable_state);
	}

	secondaryThread.join();

	stable_mutex.lock();
	if (stable_state != nullptr)
		delete stable_state;
	stable_mutex.unlock();

	deleteBuffers();

	glfwDestroyWindow(window);

	glDeleteVertexArrays(1, &skybox.VAO);
	glDeleteBuffers(1, &skybox.VBO);
	glDeleteBuffers(1, &skybox.EBO);

	glDeleteTextures(textureArray.depth, &textureArray.id);
	glDeleteTextures(1, &skybox.textureID);

	glDeleteProgram(skybox.getShaderID());
	glDeleteProgram(ourShader.ID);

	glfwTerminate();

	return 0;
}

void	draw(GLFWwindow* window, obj::Shader &ourShader, obj::Skybox &skybox, std::vector<obj::Chunk *>	*stable_state)
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

	stable_mutex.lock();
	if (stable_state != nullptr)
	{
		for (auto chunk: *stable_state)
		{
			if (chunk->isCreated == false)
				continue;
			if (chunk->isVAO == false)
				chunk->setupMesh();
			chunk->draw(ourShader);
		}
	}
	stable_mutex.unlock();

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ft_vox", NULL, NULL);
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
	// enable modes
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	return window;
}

std::map<std::string, bool>	key_map;
float speed = 0.1f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << std::endl;
}

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
		key_map["b"] = true;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && key_map["b"])
	{
		key_map["b"] = false;
		skybox_active = !skybox_active;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		key_map["shift"] = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && key_map["shift"])
	{
		key_map["shift"] = false;
		speed = 2.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		key_map["control"] = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && key_map["control"])
	{
		speed = 0.1f;
		key_map["control"] = false;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		key_map["1"] = true;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && key_map["1"])
	{
		renderDistance = 10;
		key_map["1"] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		key_map["2"] = true;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && key_map["2"])
	{
		renderDistance = 2;
		key_map["2"] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		key_map["5"] = true;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE && key_map["5"])
	{
		renderDistance = 5;
		key_map["5"] = true;
	}

	glfwSetKeyCallback(window, key_callback);
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
