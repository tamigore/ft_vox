#ifndef __2DCHUNK_HPP__
# define __2DCHUNK_HPP__

# include <vector>
# include <map>

# include "Chunk.hpp"

typedef enum
{
	CONTINENT,
	ELEVATION,
	TEMPERATURE,
	HUMIDITY
}	NoiseType;

class ChunkGenerator
{
public:
	ChunkGenerator();
	ChunkGenerator(u_int seed);
	~ChunkGenerator();

	// void			create3DNoise( int x, int y, int z, int seed, int size);
	// void			create2DNoise( int x, int y, int seed, int size);
	// float			getchunk(int x, int y);
	obj::Chunk	*generateChunk(int posX, int posY);

private:
	// std::map<<std::pair<int, int, int>>,float>	3DNoise;
	// std::map<<std::pair<int, int>>,float>		2DNoise;
	std::vector<obj::Noise *>		noises;

	float			oceanLevel = 0.3;
	float			landLevel = 0.6;
	float			mountainLevel = 0.9;
	float			continentNoise = 0.5;
};

#endif
