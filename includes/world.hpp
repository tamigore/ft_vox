#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include "noise.hpp"

class World
{
	public:
		World();
		~World();

	private:
		std::vector<obj::Noise *> noises;
};

#endif
