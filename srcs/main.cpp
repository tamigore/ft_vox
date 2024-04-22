#include "../includes/glad/glad.h"
#include "../includes/objects/shader.hpp"
#include "../includes/objects/camera.hpp"
#include "../includes/objects/mesh.hpp"

#include "../includes/stb_image.h"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

bool	rotate = true;
bool	useColor = true;
bool	skybox_active = false;

float	mixValue = 0.2f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
obj::camera camera(math::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(int ac, char **av)
{
	if (ac != 2)
		return -1;

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

	// build and compile our shader zprogram
	obj::shader ourShader("srcs/objects/textures/texture.vs", "srcs/objects/textures/texture.fs", nullptr);

	obj::mesh mesh;
	if (!mesh.loadMesh(av[1]))
	{
		std::cout << "Failed to load mesh" << std::endl;
		return -1;
	}
	mesh.add_texture("container.png", "srcs/objects/textures");
	mesh.add_texture("awesomeface.png", "srcs/objects/textures");
	// std::cout << mesh << std::endl;

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
	// std::vector<const char *> files = {
	// 	"srcs/objects/skybox/right.png",
	// 	"srcs/objects/skybox/left.png",
	// 	"srcs/objects/skybox/top.png",
	// 	"srcs/objects/skybox/bottom.png",
	// 	"srcs/objects/skybox/front.png",
	// 	"srcs/objects/skybox/back.png"
	// };
	std::vector<const char *> files = {
		"srcs/objects/skybox/px.png",
		"srcs/objects/skybox/nx.png",
		"srcs/objects/skybox/py.png",
		"srcs/objects/skybox/ny.png",
		"srcs/objects/skybox/pz.png",
		"srcs/objects/skybox/nz.png"
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

		ourShader.setFloat("mixValue", mixValue);
		ourShader.setBool("useColor", useColor);

		math::mat4 model = math::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		if (rotate)
		{
			float angle = glfwGetTime() * 20.0f;
			math::vec3 axis = math::vec3(1.0f, 1.3f, 0.0f);
			axis.normalize();
			model = math::rotate(model, math::radians(angle), axis);
		}
		ourShader.setMat4("model", model);

		// draw mesh
		mesh.draw(ourShader);

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

bool r_pressed = false;
bool c_pressed = false;
bool b_pressed = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(obj::RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		std::cout << "UP mixValue: " << mixValue << std::endl;
		mixValue += 0.01f;
		if(mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
	{
		std::cout << "Down mixValue: " << mixValue << std::endl;
		mixValue -= 0.01f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		c_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && c_pressed)
	{
		c_pressed = false;
		useColor = !useColor;
		std::cout << "useColor: " << useColor << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		r_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE && r_pressed)
	{
		r_pressed = false;
		rotate = !rotate;
		std::cout << "rotate: " << rotate << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		b_pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && b_pressed)
	{
		b_pressed = false;
		skybox_active = !skybox_active;
		std::cout << "rotate: " << rotate << std::endl;
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
