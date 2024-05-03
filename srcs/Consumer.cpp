#include "Consumer.hpp"

// Consumer::Consumer(std::mutex &chunk_mutex): data_mutex(chunk_mutex) {}

// Consumer::~Consumer() {}

// void	Consumer::consume(std::map<std::pair<int, int>, obj::Chunk *> *chunksMap, int renderDistance, math::vec3 cameraPos)
// {
// 	std::map<std::pair<int, int>, obj::Chunk *>::iterator	chunk;
// 	std::vector<std::pair<int, int>>						set;
// 	data_mutex.lock();
// 	for (chunk = chunksMap->begin(); chunk != chunksMap->end(); chunk++)
// 	{
// 		if (std::abs(chunk->first.first - cameraPos.x / 16) > renderDistance ||
// 			std::abs(chunk->first.second - cameraPos.z / 16) > renderDistance)
// 		{
// 			set.push_back(chunk->first);
// 		}
// 	}
// 	for (auto key : set) { 
// 		delete (*chunksMap)[key];
// 		chunksMap->erase(key); 
// 	} 
// 	data_mutex.unlock();
// }