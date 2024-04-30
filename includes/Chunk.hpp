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

	void	generateChunk(int posX, int posY, Noise noise);
	void	generateFaces(void);
	void	setupMesh();
	void	draw(Shader &shader);

	bool 	isVAO=false;
	int		posX;
	int		posY;

private:
	const int	size_x = 16;
	const int	size_y = 16;
	const int	size_z = 256;

	unsigned int VAO, VBO, EBO;

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	unsigned char				*chunk;

	void	createFaces( int x, int y, int z, int position, int face, int block);
};

}

#endif
