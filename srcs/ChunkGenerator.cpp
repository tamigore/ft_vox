#include "ChunkGenerator.hpp"
#include "objects/Block.hpp"
#include "math/spline.h"

#include <thread>

ChunkGenerator::ChunkGenerator() {}

ChunkGenerator::ChunkGenerator(u_int seed)
{
	std::cout << "Generator created" << std::endl;
	noises.push_back(obj::Noise(seed, 16));
	noises.push_back(obj::Noise(seed + 1, 16));
	noises.push_back(obj::Noise(seed + 2, 16));
	noises.push_back(obj::Noise(seed + 3, 16));
}

ChunkGenerator::~ChunkGenerator() {}

int		ChunkGenerator::bedrockGenerator(float x, float y)
{
	float berdrock_noise = noises[1].Generate2D(math::vec2(x, y), 0.0423234245, 1, 0.5, 1);
	return (1 + 10 * std::abs(berdrock_noise));
}

int		ChunkGenerator::continentaleGenerator(float x, float y)
{
	tk::spline	spline;
	spline.set_points(	{-1,	-0.8,	-0.3,	-0.25,	0.01,	0.05,	0.1,	0.15,	0.6,	0.8,	1},
						{250,	30,		35,		80,		85,		150,	155,	170,	180,	190,	250});
	float	noise = noises[0].Generate2D(math::vec2(x, y), 0.00087, 2.5, 0.56, 4);
	return spline(noise);
}

int		ChunkGenerator::erosionGenerator(float x, float y)
{
	tk::spline	spline;
	spline.set_points(	{-1,	-0.75,	-0.3,	-0.25,	0.0,	0.3,	0.4,	0.45,	0.55,	0.6,	0.75,	1},
						{250,	170,	150,	160,	50,		35,		30,		70,		60,		30,		15,		10});
	float	noise = noises[1].Generate2D(math::vec2(x, y), 0.00167, 2.5, 0.5, 4);
	return spline(noise);
}

int		ChunkGenerator::peaksGenerator(float x, float y)
{
	tk::spline	spline;
	spline.set_points(	{-1,	-0.8,	-0.5,	-0.25,	0.0,	0.2,	0.5,	0.65,	0.8,	1},
						{0,		20,		35,		45,		50,		80,		160,	200,	220,	250});
	float	noise = noises[2].Generate2D(math::vec2(x, y), 0.00367, 2.5, 0.75, 4);
	return spline(noise);
}

int		ChunkGenerator::terrainHightGenerator(float x, float y)
{
	int continent = continentaleGenerator(x, y);
	int erosion = erosionGenerator(x, y);
	int peaks = peaksGenerator(x, y);
	return ((continent + erosion + peaks) / 3);
}

float		ChunkGenerator::caveGenerator(float x, float y, float z)
{
	float cave_noise = noises[0].Generate3D(math::vec3(x, y, z), 0.02, 1.65, 0.5, 4);
	return (cave_noise);
}

obj::Chunk	*ChunkGenerator::generateChunk(int posX, int posY)
{
	obj::Chunk *res = new obj::Chunk(posX, posY);
	float px = res->size_x;
	float py = res->size_y, pz = res->size_z;
	unsigned int block = 0;

	for (int x = 0; x < res->size_x; x++)
	{
		px = posX * res->size_x + x;
		for (int y = 0; y < res->size_y; y++)
		{
			py = posY * res->size_y + y;
			int bedrock = bedrockGenerator(px, py);
			int terrainHight = terrainHightGenerator(px, py);
			for (int z = 0; z < res->size_z; z++)
			{
				block = z + y * res->size_z + x * res->size_y * res->size_z;
				res->chunk[block] = BlockType::air;

				if (z < bedrock)
					res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::bedrock;
				else if (z < terrainHight)
				{
					if (z < terrainHight - 1)
					{
						if (z < terrainHight - (bedrock * 2))
						{
							res->chunk[block] = BlockType::stone;
							if (z < terrainHight - 10 - (bedrock * 2))
							{
								float big = caveGenerator(px, py, z);
								if (big > 0.5)
									res->chunk[block] = BlockType::air;
							}
						}
						else
						{
							if (z < 70 + bedrock)
								res->chunk[block] = BlockType::stone;
							else if (z < 75 + bedrock)
								res->chunk[block] = BlockType::gravel;
							else
								res->chunk[block] = BlockType::dirt;
						}
					}
					else
					{
						if (z < 75 + bedrock)
							res->chunk[block] = BlockType::gravel;
						else if (z < 81 + bedrock)
							res->chunk[block] = BlockType::sand;
						else if (z < 175 + bedrock)
							res->chunk[block] = BlockType::grass;
						else
							res->chunk[block] = BlockType::snow;
					}
				}
				else if (z < 80)
					res->chunk[block] = BlockType::water;
			}
		}
	}
	return res;
}
