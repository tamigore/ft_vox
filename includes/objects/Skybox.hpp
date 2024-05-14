#ifndef __SKYBOX_HPP__
# define __SKYBOX_HPP__

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "objects/Shader.hpp"
#include "Utils.hpp"

#include <GLFW/glfw3.h>

namespace obj
{

class Skybox
{
	public:
		Skybox();
		~Skybox();

		void	draw(math::mat4 view, math::mat4 projection);
		void	load();
		void	setup();

		unsigned int	getShaderID() { return skyboxShader.ID; }
		unsigned int	getTextureID() { return textureID; }

		unsigned int	VAO, VBO, EBO;
		unsigned int	textureID;

	private:
		obj::Shader		skyboxShader;

};

}

#endif
