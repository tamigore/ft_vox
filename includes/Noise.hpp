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

		double	fade(double t);
		double	lerp(double t, double a, double b);
		double	grad(int hash, double x, double y, double z);
		double	noise(double x, double y, double z);

	private:
		uint seed;
		uint size = 16;
		std::vector<uint> tab;
};

}

#endif
