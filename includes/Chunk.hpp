#ifndef Chunk_HPP
# define Chunk_HPP

# include <vector>

# include "objects/Vertex.hpp"
# include "objects/Shader.hpp"
# include "objects/Texture.hpp"
# include "math/math.hpp"
# include "Noise.hpp"
# include "glad/glad.h"

namespace obj
{

class Chunk
{
public:
	Chunk(int x, int y);
	~Chunk();

	void			createFaces( int x, int y, int z, int position, int face, int block);
	void			draw(Shader &shader);
	// void			initVAO();
	// bool			add_texture(const char *name, const char *path);
	// unsigned int	TextureFromFile(const char *path, const std::string &directory);
	void			setupMesh();

	bool 	isVAO=false;
	int		posX;
	int		posY;

private:

	unsigned int VAO, VBO, EBO;

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;
};

}

#endif
