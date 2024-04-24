#include "../includes/glad/glad.h"
#include "../includes/objects/shader.hpp"
#include "../includes/objects/camera.hpp"
#include "../includes/objects/mesh.hpp"
#include "../includes/chunk.hpp"
#include "../includes/noise.hpp"

#include "../includes/stb_image.h"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
bool	skybox_active = false;

float	mixValue = 0.2f;
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
		return -1;
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
		return -1;
	}

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
	obj::shader skyboxShader("srcs/objects/skybox/skybox.vs", "srcs/objects/skybox/skybox.fs", nullptr);

	// COORDINATES
	float skyboxVertices[] = {
		-1.0f,	-1.0f,	 1.0f,//        7--------6
		 1.0f,	-1.0f,	 1.0f,//       /|       /|
		 1.0f,	-1.0f,	-1.0f,//      4--------5 |
		-1.0f,	-1.0f,	-1.0f,//      | |      | |
		-1.0f,	 1.0f,	 1.0f,//      | 3------|-2
		 1.0f,	 1.0f,	 1.0f,//      |/       |/
		 1.0f,	 1.0f,	-1.0f,//      0--------1
		-1.0f,   1.0f,	-1.0f
	};

	// INDICES
	unsigned int skyboxIndices[] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	// // SKY BOX IMAGES
	std::vector<const char *> files = {
		"srcs/objects/skybox/cloudySky/px.png",
		"srcs/objects/skybox/cloudySky/nx.png",
		"srcs/objects/skybox/cloudySky/py.png",
		"srcs/objects/skybox/cloudySky/ny.png",
		"srcs/objects/skybox/cloudySky/pz.png",
		"srcs/objects/skybox/cloudySky/nz.png"
	};

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < files.size(); i++)
	{
		std::cout << files[i] << std::endl;

		int width, height, nrChannels;
		unsigned char* data = stbi_load(files[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << files[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// render loop
	math::mat4 model = math::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = math::rotate(model, math::radians(90.0f), math::vec3(1.0f, 0.0f, 0.0f));
	model = model.translate(-10.0f, -15.0f, -5.0f);
	std::vector<obj::Chunk *> toDrawChunks;
	std::thread thread(update_chunk, renderDistance, &toDrawChunks);
	while (!glfwWindowShouldClose(window))
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
			// draw skybox as last
			glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_FALSE);

			skyboxShader.use();
			view[3][0] = 0;
			view[3][1] = 0;// a.x a.y a.z 0
			view[3][2] = 0;// b.x b.y b.z 0
			view[3][3] = 0;// c.x c.y c.z 0
			view[0][3] = 0;// 0   0   0   0
			view[1][3] = 0;
			view[2][3] = 0;
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

			skyboxShader.use();
			skyboxShader.setMat4("projection", projection);
			skyboxShader.setMat4("view", view);
		
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
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
