#ifndef NOISE_HPP
# define NOISE_HPP

# include <vector>

# include "math/math.hpp"

namespace obj
{

class Noise
{
	public:
		Noise();
		Noise(uint seed);
		Noise(uint seed, uint size);
		~Noise();

		float	Generate2D(math::vec2 pos, float scale, float amplitude, float persistence, int octaves);
		float	Generate3D(math::vec3 pos, float scale, float amplitude, float persistence, int octaves);

	private:
		double	noise(double x, double y, double z);
		
		uint				seed;
		uint				size = 16;
		std::vector<uint>	tab;
};

}

#endif
