#include "../includes/glad/glad.h"
#include "../includes/objects/shader.hpp"
#include "../includes/objects/camera.hpp"
#include "../includes/objects/mesh.hpp"
#include "../includes/objects/noise.hpp"

#include <unistd.h>
#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// void processInput(GLFWwindow *window);

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// // camera
// obj::camera camera(math::vec3(0.0f, 0.0f, 3.0f));
// float lastX = SCR_WIDTH / 2.0f;
// float lastY = SCR_HEIGHT / 2.0f;
// bool firstMouse = true;

// // stores how much we're seeing of either texture
// float mixValue = 0.2f;

// // timing
// float deltaTime = 0.0f;	// time between current frame and last frame
// float lastFrame = 0.0f;

// int main()
// {
//     // glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     // glfw window creation
//     // --------------------
//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scop", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     // glad: mouse lock and callbacks
//     glfwSetCursorPosCallback(window, mouse_callback);
//     glfwSetScrollCallback(window, scroll_callback);
//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     // glad: load all OpenGL function pointers
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // build and compile our shader zprogram
//     // obj::shader ourShader("srcs/objects/shaders/shader.vs", "srcs/objects/shaders/shader.fs");
//     obj::shader ourShader("srcs/objects/textures/texture.vs", "srcs/objects/textures/texture.fs");

// 	float vertices[] = {
//     // positions          // colors           // texture coords
//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

//     -0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

//     -0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//     -0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

//     -0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f
// 	};

//     unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     // position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     // color attribute
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);
//     // texture coord attribute
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//     glEnableVertexAttribArray(2);


//     // load and create a texture 
//     unsigned int texture1, texture2;
//     // texture 1
//     glGenTextures(1, &texture1);
//     glBindTexture(GL_TEXTURE_2D, texture1);
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // set texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     // load image, create texture and generate mipmaps
//     int width, height, nrChannels;
//     stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//     // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//     unsigned char *data = stbi_load("srcs/objects/textures/container.png", &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture 1" << std::endl;
//     }
//     stbi_image_free(data);
//     // texture 2
//     glGenTextures(1, &texture2);
//     glBindTexture(GL_TEXTURE_2D, texture2);
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     // set texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 	// load image, create texture and generate mipmaps
//     data = stbi_load("srcs/objects/textures/awesomeface.png", &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture 2" << std::endl;
//     }
//     stbi_image_free(data);

//     // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
//     ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
//     // either set it manually like so:
//     glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
//     // or set it via the texture class
//     ourShader.setInt("texture2", 1);

// 	std::vector<math::vec3> cubePositions = {
// 		math::vec3( 0.0f,  0.0f,  0.0f), 
// 		math::vec3( 2.0f,  5.0f, -15.0f), 
// 		math::vec3(-1.5f, -2.2f, -2.5f),  
// 		math::vec3(-3.8f, -2.0f, -12.3f),  
// 		math::vec3( 2.4f, -0.4f, -3.5f),  
// 		math::vec3(-1.7f,  3.0f, -7.5f),  
// 		math::vec3( 1.3f, -2.0f, -2.5f),  
// 		math::vec3( 1.5f,  2.0f, -2.5f), 
// 		math::vec3( 1.5f,  0.2f, -1.5f), 
// 		math::vec3(-1.3f,  1.0f, -1.5f)  
// 	};

// 	// bind textures on corresponding texture units
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, texture1);
// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, texture2);
	
// 	ourShader.use();

// 	// setup model view and projection matrices
// 	math::mat4 model = math::mat4(1.0f);
// 	math::vec3 rotdir = math::vec3(1.0f, 0.3f, 0.5f);
// 	rotdir.normalize();
// 	// view  = math::translate(view, math::vec3(0.0f, 0.0f, -3.0f));
// 	math::mat4 projection = math::perspective(math::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
// 	ourShader.setMat4("projection", projection);

// 	// bind model view and projection matrices in opengl
// 	unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
// 	unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
// 	std::cout << "View matrix:" << std::endl;
// 	math::mat4 view = camera.GetViewMatrix();

// 	// set backgroud color
// 	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     // render loop
//     while (!glfwWindowShouldClose(window))
//     {
// 		// per-frame time logic
//         // --------------------
//         float currentFrame = static_cast<float>(glfwGetTime());
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;
//         // input
//         processInput(window);

// 		// clear color buffer
// 		glClear(GL_COLOR_BUFFER_BIT);

// 		// enable depth testing
// 		glEnable(GL_DEPTH_TEST);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		// set the matrices for viewpoint and model transformation
// 		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
// 		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
// 		// set the texture mix value in the shader
// 		ourShader.setFloat("mixValue", mixValue);
		
// 		// render container
//         glBindVertexArray(VAO);
// 		for(unsigned int i = 0; i < cubePositions.size(); i++)
// 		{
// 			math::mat4 model = math::mat4(1.0f); // make sure to initialize matrix to identity matrix first
// 			model = math::translate(model, cubePositions[i]);
// 			float angle = (float)glfwGetTime() * 20.0f * i;
// 			model = math::rotate(model, math::radians(angle), math::vec3(1.0f, 0.3f, 0.5f));
// 			ourShader.setMat4("model", model);

// 			glDrawArrays(GL_TRIANGLES, 0, 36);
			
// 			// math::mat4 model = math::mat4(1.0f);
// 			// model = math::translate(model, cubePositions[i]);
// 			// float angle = 20.0f * i; 
// 			// model = math::rotate(model, math::radians((float)glfwGetTime() * (angle)), rotdir);
// 			// ourShader.setMat4("model", model);

// 			// glDrawArrays(GL_TRIANGLES, 0, 36);
// 	        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 		}

//         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     // optional: de-allocate all resources once they've outlived their purpose:
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);

//     // glfw: terminate, clearing all previously allocated GLFW resources.
//     glfwTerminate();
//     return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);

//     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//     {
// 		std::cout << "UP mixValue: " << mixValue << std::endl;
//         mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
//         if(mixValue >= 1.0f)
//             mixValue = 1.0f;
//     }
//     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//     {
// 		std::cout << "Down mixValue: " << mixValue << std::endl;
//         mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
//         if (mixValue <= 0.0f)
//             mixValue = 0.0f;
//     }

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 	{
// 		std::cout << "GLFW_KEY_W: " << deltaTime << std::endl;
//         camera.ProcessKeyboard(obj::FORWARD, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     {
// 		std::cout << "GLFW_KEY_S: " << deltaTime << std::endl;
// 	    camera.ProcessKeyboard(obj::BACKWARD, deltaTime);
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 	{
// 		std::cout << "GLFW_KEY_A: " << deltaTime << std::endl;
// 		camera.ProcessKeyboard(obj::LEFT, deltaTime);
// 	}
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     {
// 		std::cout << "GLFW_KEY_D: " << deltaTime << std::endl;
// 		camera.ProcessKeyboard(obj::RIGHT, deltaTime);
// 	}
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
// 	(void)window;
//     // make sure the viewport matches the new window dimensions; note that width and 
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

// // glfw: whenever the mouse moves, this callback is called
// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
// {
// 	(void)window;

// 	float xpos = static_cast<float>(xposIn);
// 	float ypos = static_cast<float>(yposIn);

// 	if (firstMouse)
// 	{
// 	lastX = xpos;
// 	lastY = ypos;
// 	firstMouse = false;
// 	}

// 	float xoffset = xpos - lastX;
// 	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

// 	lastX = xpos;
// 	lastY = ypos;

// 	camera.ProcessMouseMovement(xoffset, yoffset, true);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
// 	(void)window;
// 	(void)xoffset;
// 	camera.ProcessMouseScroll(static_cast<float>(yoffset));
// }




bool	rotate = true;
bool	useColor = true;
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	glfwSwapInterval(1);
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
	(void)av;
	
	mesh.add_texture("dirt.png", "srcs/objects/textures");

	// load and create a texture 
	// unsigned int texture1, texture2;

	// // texture 1
	// glGenTextures(1, &texture1);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	// // set the texture wrapping parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// // set texture filtering parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load image, create texture and generate mipmaps
	// int width, height, nrChannels;
	// // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// unsigned char *data = stbi_load("srcs/objects/textures/container.png", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// {
	// 	std::cout << "Failed to load texture" << std::endl;
	// }
	// stbi_image_free(data);

	// // texture 2
	// glGenTextures(1, &texture2);
	// glBindTexture(GL_TEXTURE_2D, texture2);
	// // set the texture wrapping parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// // set texture filtering parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load image, create texture and generate mipmaps
	// data = stbi_load("srcs/objects/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	// 	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// {
	// 	std::cout << "Failed to load texture" << std::endl;
	// }
	// stbi_image_free(data);

	// activate shader and tell opengl for each sampler to which texture unit it belongs
	// ourShader.use();
	// ourShader.setInt("texture1", 0);
	// ourShader.setInt("texture2", 1);

	// background color
	// bind textures on corresponding texture units
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	// glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_2D, texture2);

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

		// ourShader.setFloat("mixValue", mixValue);
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
