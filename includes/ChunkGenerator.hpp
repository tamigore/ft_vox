#ifndef __2DCHUNK_HPP__
# define __2DCHUNK_HPP__

# include <vector>
# include <map>

# include "objects/Chunk.hpp"

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
	int			terrainHightGenerator(float x, float y);

private:
	std::vector<obj::Noise>		noises;

	float			oceanLevel = 0.3;
	float			landLevel = 0.6;
	float			mountainLevel = 0.9;
	float			continentNoise = 0.5;

	int		bedrockGenerator(float x, float y);
	int		continentaleGenerator(float x, float y);
	int		erosionGenerator(float x, float y);
	int		peaksGenerator(float x, float y);

	float	bigCaveGenerator(float x, float y, float z);
	float	caveGenerator(float x, float y, float z);
	int		cave2DGenerator(float x, float y);
};

#endif
