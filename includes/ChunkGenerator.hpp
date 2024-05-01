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

	obj::Chunk	*generateChunk(int posX, int posY);

private:
	std::vector<obj::Noise>		noises;

	float			oceanLevel = 0.3;
	float			landLevel = 0.6;
	float			mountainLevel = 0.9;
	float			continentNoise = 0.5;

	int		continentaleGenerator(float x, float y);
	int		bedrockGenerator(float x, float y);
	void	calculateNoise(obj::Chunk *res, int x, float px, int posY);
};

#endif
