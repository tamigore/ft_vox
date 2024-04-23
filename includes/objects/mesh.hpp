#ifndef __MESH_HPP__
# define __MESH_HPP__

# include <fstream>
# include <map>
# include <vector>

# include "vertex.hpp"
# include "material.hpp"
# include "face.hpp"
# include "texture.hpp"
# include "shader.hpp"
# include "../noise.hpp"
# include "../utils.hpp"

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

namespace obj
{

class mesh
{
	private:
		unsigned int VAO, VBO, EBO;

		void	setupMesh();
		bool	add_vertex_position(std::string curline);
		bool	add_vertex_texture(std::string curline);
		bool	add_vertex_normal(std::string curline);
		bool	add_face(std::string pram);

		unsigned int	TextureFromFile(const char *path, const std::string &directory);

	public:
		std::string					name;
        std::vector<vertex>			vertices;
        std::vector<unsigned int>	indices;
        std::vector<unsigned int>	position_indices;
        std::vector<unsigned int>	normal_indices;
        std::vector<unsigned int>	texture_indices;
        std::vector<texture>		textures;
        std::vector<face>			faces;
		// material					m_material;

		mesh();
		mesh(char* path);
		~mesh();

		void	createFaces(obj::mesh &mesh, int x, int y, int z, int face);
        void	draw(shader &shader);
		bool	loadMesh(const char *path);
		bool	add_texture(const char *name, const char *path);
		void	min_max_bounds(math::vec3& min_bound, math::vec3& max_bound);
		void	center_around_orgin();
		void	facesDuplicateVertexes();
};	

std::ostream&	operator<<(std::ostream &o, const mesh &rhs);

}

#endif
