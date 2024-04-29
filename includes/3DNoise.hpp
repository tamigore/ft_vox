#ifndef __3DNOISE_HPP__
# define __3DNOISE_HPP__

#include <iostream>
#include <vector>
#include "math/math.hpp"

class Noise
{
public:
	Noise(int seed, int octaves);
	~Noise();

	float	Generate2D(math::vec2 pos, int zoom, float persistence);
	float	Generate3D(math::vec3 pos, int zoom, float persistence);
};

#endif
