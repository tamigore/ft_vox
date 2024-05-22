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

GLFWwindow *glfw_init_window(void);
void draw(GLFWwindow *window, obj::Shader &ourShader, obj::Skybox &skybox, std::vector<obj::Chunk *> *stable_state);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
bool skybox_active = true;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
obj::Camera camera(math::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

std::mutex camMutex;
int stableCamX = 0;
int stableCamY = 0;

unsigned int renderDistance = 8;
ChunkGenerator generator = ChunkGenerator(42);

// Thread
std::mutex chunk_mutex;
std::mutex stable_mutex;

std::mutex toDelete_mutex;
std::vector<obj::Chunk *> toDelete;

void generate(int x, int y, std::map<std::pair<int, int>, obj::Chunk *> *chunksMap)
{
	obj::Chunk *chunk = nullptr;

	chunk_mutex.lock();
	std::map<std::pair<int, int>, obj::Chunk *>::iterator found = chunksMap->find(std::make_pair(x, y));
	if (found == chunksMap->end())
	{
		chunk = generator.generateChunk(x, y);
		found = chunksMap->find(std::make_pair(x - 1, y));
		std::lock_guard<std::mutex> lock(chunk->mutex);
		if (found != chunksMap->end())
		{
			std::lock_guard<std::mutex> next_lock(found->second->mutex);
			chunk->setWest(found->second);
			found->second->setEast(chunk);
		}
		found = chunksMap->find(std::make_pair(x + 1, y));
		if (found != chunksMap->end())
		{
			std::lock_guard<std::mutex> next_lock(found->second->mutex);
			chunk->setEast(found->second);
			found->second->setWest(chunk);
		}
		found = chunksMap->find(std::make_pair(x, y - 1));
		if (found != chunksMap->end())
		{
			std::lock_guard<std::mutex> next_lock(found->second->mutex);
			chunk->setSouth(found->second);
			found->second->setNorth(chunk);
		}
		found = chunksMap->find(std::make_pair(x, y + 1));
		if (found != chunksMap->end())
		{
			std::lock_guard<std::mutex> next_lock(found->second->mutex);
			chunk->setNorth(found->second);
			found->second->setSouth(chunk);
		}
		(*chunksMap)[std::make_pair(x, y)] = chunk;
	}
	chunk_mutex.unlock();
}

void erase_chunk_from_map(std::map<std::pair<int, int>, obj::Chunk *> *chunksMap, int camX, int camY)
{
	std::map<std::pair<int, int>, obj::Chunk *>::iterator chunk;
	std::lock_guard<std::mutex> lock(chunk_mutex);
	for (chunk = chunksMap->begin(); chunk != chunksMap->end(); chunk++)
	{
		if ((chunk->first.first < (camX - (int)renderDistance)) ||
			(chunk->first.first > (camX + (int)renderDistance)) ||
			(chunk->first.second < (camY - (int)renderDistance)) ||
			(chunk->first.second > (camY + (int)renderDistance)))
		{
			std::lock_guard<std::mutex> del_lock(toDelete_mutex);
			toDelete.push_back(chunk->second);
			chunksMap->erase(chunk);
		}
	}
}

void delete_chunk_from_toDelete(std::vector<obj::Chunk *> *stable_state)
{
	std::lock_guard<std::mutex> del_lock(toDelete_mutex);
	if (!toDelete.size())
		return;
	std::cout << "Delete Chunk" << std::endl;
	for (std::vector<obj::Chunk *>::iterator chunk = toDelete.begin(); chunk != toDelete.end(); chunk++)
	{
		if ((*chunk) == nullptr)
		{
			std::cout << "Chunk NULL" << std::endl;
			return;
		}
		std::lock_guard<std::mutex> lock((*chunk)->mutex);
		std::cout << "( " << (*chunk)->posX << " | " << (*chunk)->posY << " )" << std::endl;
		if (stable_state != nullptr)
		{
			std::lock_guard<std::mutex> stable_lock(stable_mutex);
			if (std::find(stable_state->begin(), stable_state->end(), (*chunk)) != stable_state->end())
			{
				std::cout << "Chunk still in stable state" << std::endl;
				return;
			}
		}
		if ((*chunk)->isVAO)
		{
			const GLuint vao = (*chunk)->getVAO();
			const GLuint vbo = (*chunk)->getVBO();
			const GLuint ebo = (*chunk)->getEBO();
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}
		obj::Chunk *west = (*chunk)->getWest();
		if (west)
		{
			std::lock_guard<std::mutex> next_lock(west->mutex);
			west->setEast(nullptr);
		}
		obj::Chunk *east = (*chunk)->getEast();
		if (east)
		{
			std::lock_guard<std::mutex> next_lock(east->mutex);
			east->setWest(nullptr);
		}
		obj::Chunk *north = (*chunk)->getNorth();
		if (north)
		{
			std::lock_guard<std::mutex> next_lock(north->mutex);
			north->setSouth(nullptr);
		}
		obj::Chunk *south = (*chunk)->getSouth();
		if (south)
		{
			std::lock_guard<std::mutex> next_lock(south->mutex);
			south->setNorth(nullptr);
		}
		delete (*chunk);
	}
	toDelete.clear();
}

void delete_all_chunk(std::map<std::pair<int, int>, obj::Chunk *> *chunksMap)
{
	std::map<std::pair<int, int>, obj::Chunk *>::iterator chunk;
	chunk_mutex.lock();
	for (chunk = chunksMap->begin(); chunk != chunksMap->end(); chunk++)
	{
		std::lock_guard<std::mutex> lock(chunk->second->mutex);
		toDelete.push_back(chunk->second);
	}
	chunksMap->clear();
	chunk_mutex.unlock();
}

void update_thread(std::vector<obj::Chunk *> **stable_state, GLFWwindow *window)
{
	std::map<std::pair<int, int>, obj::Chunk *> chunksMap;
	std::vector<obj::Chunk *> *tmp_state = new std::vector<obj::Chunk *>;

	std::vector<std::thread> generation;
	std::vector<std::pair<int, int>> buffer;
	int camX, camY;
	int g_pos = 0;
	int last_x = 13213;
	int last_y = 3213;

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

		erase_chunk_from_map(&chunksMap, camX, camY);
	}
	delete_all_chunk(&chunksMap);
	if (tmp_state != nullptr)
		delete tmp_state;
}

int main()
{
	GLFWwindow *window = glfw_init_window();
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

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
	std::vector<obj::Chunk *> *stable_state = new std::vector<obj::Chunk *>;
	std::thread secondaryThread(update_thread, &stable_state, window);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		camMutex.lock();
		stableCamX = camera.Position.x;
		stableCamY = camera.Position.z;
		camMutex.unlock();
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.id);
		draw(window, ourShader, skybox, stable_state);
		delete_chunk_from_toDelete(stable_state);
	}

	secondaryThread.join();

	stable_mutex.lock();
	if (stable_state != nullptr)
		delete stable_state;
	stable_mutex.unlock();

	delete_chunk_from_toDelete(nullptr);

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

void draw(GLFWwindow *window, obj::Shader &ourShader, obj::Skybox &skybox, std::vector<obj::Chunk *> *stable_state)
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

	if (stable_state != nullptr)
	{
		std::lock_guard<std::mutex> stable_lock(stable_mutex);
		for (std::vector<obj::Chunk *>::iterator chunk = stable_state->begin(); chunk != stable_state->end(); chunk++)
		{
			// std::cout << "1" << std::endl;
			if (!(*chunk))
			{
				// std::cout << "if" << std::endl;
				continue;
			}
			// std::cout << "2" << std::endl;
			std::lock_guard<std::mutex> lock((*chunk)->mutex);
			// std::cout << "3" << std::endl;
			if ((*chunk)->isCreated == false)
				continue;
			if ((*chunk)->isVAO == false)
				(*chunk)->setupMesh();
			(*chunk)->draw();
			// std::cout << "4" << std::endl;
		}
	}

	if (skybox_active)
		skybox.draw(view, projection);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow *glfw_init_window(void)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ft_vox", NULL, NULL);
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

	return window;
}

std::map<std::string, bool> key_map;
float speed = 0.1f;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
// 	std::cout << key << std::endl;
// }

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

	// glfwSetKeyCallback(window, key_callback);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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

	camera.ProcessMouseMovement(xoffset * 0.1, yoffset * 0.1, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
