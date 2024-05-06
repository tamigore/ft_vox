#ifndef __WORLD_HPP__
# define __WORLD_HPP__

# include <vector>
# include <map>

# include "objects/Camera.hpp"
# include "objects/Skybox.hpp"
# include "objects/Noise.hpp"
# include "objects/Chunk.hpp"

class World
{
	public:
		World();
		~World();

		bool						skybox_active;
		int							renderDistance;
		obj::Skybox					skybox;
		std::vector<obj::Noise *>	noises;
		std::vector<obj::Chunk *>	chunks;
		std::map<std::pair<int, int>, obj::Chunk *> chunksMap;

	private:
};

#endif
