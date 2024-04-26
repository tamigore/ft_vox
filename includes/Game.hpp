#ifndef __GAME_HPP__
# define __GAME_HPP__

# include <map>
# include <iostream>
# include <thread>
# include <mutex>

# include "glad/glad.h"
# include "World.hpp"
# include "objects/Camera.hpp"

namespace game
{

// const unsigned int		SCR_WIDTH = 1920;
// const unsigned int		SCR_HEIGHT = 1080;

// float	lastX = SCR_WIDTH / 2.0f;
// float	lastY = SCR_HEIGHT / 2.0f;
// bool	firstMouse = true;
// float	deltaTime = 0.0f;
// float	lastFrame = 0.0f;

// obj::Camera	camera(math::vec3(0.0f, 0.0f, 3.0f));

class Game
{
	public:
		Game();
		~Game();

		math::mat4	projection;
		math::mat4	view;
		math::mat4	model;

		void		processInput(GLFWwindow *window);
		// void 		update_chunk(int renderDistance, std::vector<obj::Chunk *> *toDrawChunks);
		// void		threads(std::vector<obj::Chunk *> *toDrawChunks);
		void		draw(GLFWwindow *window, std::vector<obj::Chunk *> &toDrawChunks);
		// GLFWwindow*	glfw_init_window(void);

	private:

		std::map<char, bool>	keys;
		World					world;
		obj::Shader				ourShader;

		// std::mutex map_mutex;
		// std::mutex	vector_mutex;
};

}


#endif
