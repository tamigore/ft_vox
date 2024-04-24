#ifndef WORLD_HPP
# define WORLD_HPP

# include <vector>
# include <map>

# include "objects/camera.hpp"
# include "noise.hpp"
# include "chunk.hpp"

class World
{
	public:
		World();
		~World();

	private:
		bool					skybox_active = true;
		const unsigned int		SCR_WIDTH = 1920;
		const unsigned int		SCR_HEIGHT = 1080;
		float lastX = SCR_WIDTH / 2.0f;
		float lastY = SCR_HEIGHT / 2.0f;
		bool firstMouse = true;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		std::map<char, bool>	keys;

		obj::camera					camera;
		std::vector<obj::Noise *>	noises;
		std::vector<obj::Chunk *>	chunks;
};

#endif
