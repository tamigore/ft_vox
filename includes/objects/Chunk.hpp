#ifndef Chunk_HPP
# define Chunk_HPP

# include <vector>
# include <mutex>
# include <memory>

# include "objects/Vertex.hpp"
# include "objects/Shader.hpp"
# include "objects/Texture.hpp"
# include "objects/Block.hpp"
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

	public:
		int		posX;
		int		posY;

		const int	size_x = 16;
		const int	size_y = 16;
		const int	size_z = 256;

		std::unique_ptr<unsigned char []>	chunk;

		mutable std::mutex		mutex;

		Chunk(int x, int y);
		~Chunk();

		// Chunk(Chunk &chunk);
		// Chunk	operator=(Chunk chunk);

		void	generateFaces(void);
		void	setupMesh();
		void	draw();
		void	createFaces( int x, int y, int z, int position, int face, int block);

		bool 	isVAO=false;
		bool 	isCreated=false;

		Chunk	*getWest();
		Chunk	*getEast();
		Chunk	*getNorth();
		Chunk	*getSouth();

		void	setWest(Chunk *c);
		void	setEast(Chunk *c);
		void	setNorth(Chunk *c);
		void	setSouth(Chunk *c);

		int		getVAO() { return VAO; }
		int		getVBO() { return VBO; }
		int		getEBO() { return EBO; }

		bool	isTransparent(int position, unsigned int block);
};

}

#endif
