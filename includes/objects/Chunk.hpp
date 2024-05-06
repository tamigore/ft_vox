#ifndef Chunk_HPP
# define Chunk_HPP

# include <vector>
# include <mutex>

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
	private:
		Chunk	*west = nullptr;
		Chunk	*east = nullptr;
		Chunk	*north = nullptr;
		Chunk	*south = nullptr;

		unsigned int VAO, VBO, EBO;

		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;

		void	createFaces( int x, int y, int z, int position, int face, int block);

	public:
		Chunk(int x, int y);
		~Chunk();

		std::mutex mutex;

		void	generateFaces(void);
		void	setupMesh();
		void	draw(Shader &shader);

		bool 	isVAO=false;
		bool 	isCreated=false;

		int		posX;
		int		posY;

		const int	size_x = 16;
		const int	size_y = 16;
		const int	size_z = 256;

		unsigned char	*chunk;

		Chunk	*getWest();
		Chunk	*getEast();
		Chunk	*getNorth();
		Chunk	*getSouth();

		void	setWest(Chunk *c);
		void	setEast(Chunk *c);
		void	setNorth(Chunk *c);
		void	setSouth(Chunk *c);

		int getVAO() { return VAO; }
		int getVBO() { return VBO; }
		int getEBO() { return EBO; }

		// Chunk	*getChunk(Chunk *chunk, int x, int y);
};

}

#endif
