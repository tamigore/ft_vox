#include "Producer.hpp"

// Producer::Producer(std::mutex &chunk_mutex): data_mutex(chunk_mutex) {}

// Producer::~Producer() {}

// void	generate(int i, int j, int renderDistance, math::vec3 cameraPos, std::map<std::pair<int, int>, obj::Chunk *> chunksMap, obj::Generator generator, std::mutex &chunk_mutex)
// {
// 	int x = cameraPos.x / 16 + i - renderDistance;
// 	int y = cameraPos.z / 16 + j - renderDistance;
// 	chunk_mutex.lock();
// 	std::map<std::pair<int, int>, obj::Chunk *>::iterator found = chunksMap.find(std::make_pair(x, y));
// 	if (found == chunksMap.end())
// 	{
// 		obj::Chunk *chunk = generator.generateChunk(x, y);
// 		found = chunksMap.find(std::make_pair(x - 1, y));
// 		if (found != chunksMap.end())
// 		{
// 			chunk->setWest(found->second);
// 			found->second->setEast(chunk);
// 		}
// 		found = chunksMap.find(std::make_pair(x + 1, y));
// 		if (found != chunksMap.end())
// 		{
// 			chunk->setEast(found->second);
// 			found->second->setWest(chunk);
// 		}
// 		found = chunksMap.find(std::make_pair(x - 1, y));
// 		if (found != chunksMap.end())
// 		{
// 			chunk->setSouth(found->second);
// 			found->second->setNorth(chunk);
// 		}
// 		found = chunksMap.find(std::make_pair(x + 1, y));
// 		if (found != chunksMap.end())
// 		{
// 			chunk->setNorth(found->second);
// 			found->second->setSouth(chunk);
// 		}
// 		chunksMap[std::make_pair(x, y)] = chunk;
// 	}
// 	chunk_mutex.unlock();
// }

// void	create(obj::Chunk *chunk)
// {
// 	if (chunk.second->isCreated == true)
// 		return;

// 	unsigned int	size_x = chunk.second->size_x;
// 	unsigned int	size_y = chunk.second->size_x;
// 	unsigned int	size_z = chunk.second->size_x;

// 	for (int x = 0; x < size_x; x++){
// 		for (int y = 0; y < size_y; y++){
// 			for (int z = 0; z < size_z; z++){
// 				if (!chunk->chunk[z + y * size_z + x * size_y * size_z])
// 					continue;
// 				int position;
// 				position = (z - 1) + y * size_z + x * size_y * size_z;
// 				if (z == 0 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 0, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 				position = (z + 1) + y * size_z + x * size_y * size_z;
// 				if (z == 255 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 1, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 				position = z + (y - 1) * size_z + x * size_y * size_z;
// 				if (y == 0 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 2, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 				position = z + (y + 1) * size_z + x * size_y * size_z;
// 				if (y == 15 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 3, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 				position = z + y * size_z + (x - 1) * size_y * size_z;
// 				if (x == 0 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 4, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 				position = z + y * size_z + (x + 1) * size_y * size_z;
// 				if (x == 15 || !chunk->chunk[position])
// 					chunk->createFaces(posX * size_x + x, posY * size_y + y, z, position, 5, chunk->chunk[z + y * size_z + x * size_y * size_z]);
// 			}
// 		}
// 	}
// 	isCreated = true;
// }

// void	Producer::generateFaces(std::map<std::pair<int, int>, obj::Chunk *> *data)
// {
// 	for (int i = 0; i < chunksMap.size(); i++)
// 	{
// 		create.push_back(std::thread(create, i));
// 	}
// 	for (int i = 0; i < create.size(); i++)
// 	{
// 		create.at(i).join();
// 	}
// }

// void	Producer::generateGeometry(std::map<std::pair<int, int>, obj::Chunk *> *data)
// {
// 	for (int i = 0; i < renderDistance * 2; i++)
// 	{
// 		for (int j = 0; j < renderDistance * 2; j++)
// 			generation.push_back(std::thread(generate, i, j, renderDistance, cameraPos, chunksMap, generator, data_mutex));
// 	}
// 	for (int i = 0; i < generation.size(); i++)
// 	{
// 		generation.at(i).join();
// 	}
// }

// void	Producer::produce(std::map<std::pair<int, int>, obj::Chunk *> *data)
// {
// 	generateGeometry(std::map<std::pair<int, int>, obj::Chunk *> *data);
// 	generateFaces(std::map<std::pair<int, int>, obj::Chunk *> *data);
// }
