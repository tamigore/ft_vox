#include "ChunkGenerator.hpp"

#include "math/spline.h"
#include <thread>

typedef enum
{
	air,
	dirt,
	grass,
	stone,
	bedrock,
	sand,
	oak_wood_side,
	leaves_2,
	water,
	snow,
	iron_ore,
	gold_ore ,
	diamond_ore,
	UNKNOWN
}	BlockType;

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

int		ChunkGenerator::continentaleGenerator(float x, float y)
{
	tk::spline	spline;
	spline.set_points({-1, 0.3, 0.4, 0.9, 1}, {30, 50, 100, 150, 150});
	spline.make_monotonic();
	float	continental_noise = noises[0].Generate2D(math::vec2(x, y), 0.00567, 2, 0.75, 4);
	return spline(continental_noise);
}

int		ChunkGenerator::bedrockGenerator(float x, float y)
{
	float berdrock_noise = noises[1].Generate2D(math::vec2(x, y), 0.0423234245, 1, 0.5, 1);
	return (1 + 10 * std::abs(berdrock_noise));
}

void	ChunkGenerator::calculateNoise(obj::Chunk *res, int x, float px, int posY)
{
	for (int y = 0; y < res->size_y; y++) {
		float py = posY * res->size_y + y;
		int bedrock = bedrockGenerator(px, py);
		int continent = continentaleGenerator(px, py);
		for (int z = 0; z < res->size_z; z++)
		{
			if (z < bedrock)
				res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::bedrock;
			else if (z < continent)
				res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::stone;
			else
				res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::air;
		}
	}
}

obj::Chunk	*ChunkGenerator::generateChunk(int posX, int posY)
{
	obj::Chunk *res = new obj::Chunk(posX, posY);
	float px = res->size_x, py = res->size_y, pz = res->size_z;

	for (int x = 0; x < res->size_x; x++)
	{
		px = posX * res->size_x + x;
		std::thread t([this, res, x, px, posY]()
		{
			for (int y = 0; y < res->size_y; y++)
			{
				float py = posY * res->size_y + y;
				int bedrock = bedrockGenerator(px, py);
				int continent = continentaleGenerator(px, py);
				for (int z = 0; z < res->size_z; z++)
				{
					if (z < bedrock)
						res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::bedrock;
					else if (z < continent)
						res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::stone;
					else if (z < 100)
						res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::water;
					else
						res->chunk[z + y * res->size_z + x * res->size_y * res->size_z] = BlockType::air;
				}
			}
		});
		t.join();
	}
	return res;
}
