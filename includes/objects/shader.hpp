#ifndef __SHADER_HPP__
# define __SHADER_HPP__

# include "../glad/glad.h"

#include <glm/glm.hpp>
# include <string>
# include <fstream>
# include <sstream>
# include <iostream>
# include "../math/math.hpp"

namespace obj
{

class shader
{
	private:
		void	checkCompileErrors(unsigned int shader, std::string type);

	public:
		// the program ID
		unsigned int	ID;
	
		shader();
		shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
		~shader();

		void	use();
		void	load(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

		void	setBool(const std::string &name, bool value) const;
		void	setInt(const std::string &name, int value) const;
		void	setFloat(const std::string &name, float value) const;
		void	setVec2(const std::string &name, const math::vec2 &value) const;
		void	setVec2(const std::string &name, float x, float y) const;
		void	setVec3(const std::string &name, const math::vec3 &value) const;
		void	setVec3(const std::string &name, float x, float y, float z) const;
		void	setVec4(const std::string &name, const math::vec4 &value) const;
		void	setVec4(const std::string &name, float x, float y, float z, float w) const;
		void	setMat2(const std::string &name, const math::mat2 &mat) const;
		void	setMat3(const std::string &name, const math::mat3 &mat) const;
		void	setMat4(const std::string &name, const math::mat4 &mat) const;
};

}

#endif
