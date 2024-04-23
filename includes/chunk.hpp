#ifndef Chunk_HPP
# define Chunk_HPP

# include <vector>

# include "objects/vertex.hpp"
# include "objects/shader.hpp"
# include "objects/texture.hpp"
# include "math/math.hpp"
# include "noise.hpp"
# include "glad/glad.h"

namespace obj
{

class Chunk
{
public:
	Chunk(int x, int y);
	~Chunk();

	void	createFaces( int x, int y, int z, int position, int face, int block);
	void	draw(shader &shader);
	bool	add_texture(const char *name, const char *path);

private:

	unsigned int VAO, VBO, EBO;

	std::vector<vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<texture>		textures;

	int		posX;
	int		posY;

	void			setupMesh();
	unsigned int	TextureFromFile(const char *path, const std::string &directory);
};

}

#endif
