#ifndef __2DCHUNK_HPP__
# define __2DCHUNK_HPP__

# include <vector>

# include "objects/Vertex.hpp"
# include "objects/Shader.hpp"
# include "objects/Texture.hpp"
# include "math/math.hpp"
# include "Noise.hpp"
# include "glad/glad.h"
# include "Block.hpp"

class 2DChunk
{
public:
	Chunk(int x, int y);
	~Chunk();

	void		createFaces( int x, int y, int z, int position, int face, int block);
	void		draw(Shader &shader);
	void		setup();
	BlockType	getBlock(int x, int y, int z);

	bool 	isVAO=false;
	int		posX;
	int		posY;

	2DChunk *top;
	2DChunk *bottom;
	2DChunk *left;
	2DChunk *right;

private:

	unsigned int VAO, VBO, EBO;

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;
};

#endif
