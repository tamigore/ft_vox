#ifndef __MESH_HPP__
# define __MESH_HPP__

# include <fstream>
# include <vector>

# include "vertex.hpp"
# include "material.hpp"
# include "face.hpp"
# include "texture.hpp"
# include "shader.hpp"
# include "../utils.hpp"

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
		// std::vector<math::vec3>		vertices;
		// std::vector<math::vec3> 	normal;
		// std::vector<math::vec2> 	textures;
		// std::vector<math::vec3>	index_vertices;
		// std::vector<math::vec3> 	index_normals;
		// std::vector<math::vec2> 	index_textures;
        std::vector<vertex>			vertices;
        std::vector<unsigned int>	indices;
        std::vector<unsigned int>	position_indices;
        std::vector<unsigned int>	normal_indices;
        std::vector<unsigned int>	texture_indices;
        std::vector<texture>		textures;
        std::vector<face>			faces;
        // std::vector<math::vec3>	triangle;
		// material					m_material;

		mesh();
		mesh(char* path);
		// mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);
		~mesh();

        void	draw(shader &shader);
		bool	loadMesh(const char *path);
		bool	add_texture(const char *name, const char *path);
		void	min_max_bounds(math::vec3& min_bound, math::vec3& max_bound);
		void	center_around_orgin();
};	

std::ostream&	operator<<(std::ostream &o, const mesh &rhs);

}

#endif
