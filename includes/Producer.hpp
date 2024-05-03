#ifndef __PRODUCER_HPP__
# define __PRODUCER_HPP__

#include <map>
#include <mutex>

# include "Chunk.hpp"

// class Producer
// {
// 	public:
// 		std::mutex	&data_mutex;
// 		int			renderDistance;
// 		math::vec3	cameraPos;

// 		Producer(std::mutex &chunk_mutex, int renderDistance, math::vec3 cameraPos);
// 		~Producer();

// 		void	produce(std::map<std::pair<int, int>, obj::Chunk *> *data);
	
// 	private:
// 		void	generateGeometry();
// 		void	generateFaces()
// };

#endif
