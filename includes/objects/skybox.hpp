#ifndef __SKYBOX_HPP__
# define __SKYBOX_HPP__

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "objects/shader.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>

namespace obj
{



class Skybox
{
	public:
		Skybox();
		~Skybox();

		void	draw(shader &shader, math::mat4 view, math::mat4 projection);
		void	load();
		void	setup();

	private:
		unsigned int	VAO, VBO, EBO;
		unsigned int	textureID;

};

}

#endif
