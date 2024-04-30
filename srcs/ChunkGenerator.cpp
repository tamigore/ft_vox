#include "ChunkGenerator.hpp"


ChunkGenerator::ChunkGenerator()
{
}

ChunkGenerator::ChunkGenerator(u_int seed)
{
	std::cout << "Generator created" << std::endl;
	noises.push_back(new obj::Noise(seed, 16));
	noises.push_back(new obj::Noise(seed + 1, 16));
	noises.push_back(new obj::Noise(seed + 2, 16));
	noises.push_back(new obj::Noise(seed + 3, 16));
}

ChunkGenerator::~ChunkGenerator()
{
}

obj::Chunk	*ChunkGenerator::generateChunk(int posX, int posY)
{
	obj::Chunk *res = new obj::Chunk(posX, posY);
	res->generateChunk(posX, posY, *(noises[0]));
	res->generateFaces();
	// res->setupMesh();
	return res;
}
