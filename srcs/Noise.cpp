#include "objects/Noise.hpp"

#include <numeric>
#include <random>
#include <algorithm>

using namespace obj;

Noise::Noise() {}

Noise::~Noise() {}

Noise::Noise(uint seed) : seed(seed)
{
	tab.resize(size * size);
	std::iota(tab.begin(), tab.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(tab.begin(), tab.end(), engine);
	tab.insert(tab.end(), tab.begin(), tab.end());
}

Noise::Noise(uint seed, uint input_size) : seed(seed)
{
	size = input_size;
	tab.resize(size * size);
	std::iota(tab.begin(), tab.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(tab.begin(), tab.end(), engine);
	tab.insert(tab.end(), tab.begin(), tab.end());
}

double	Noise::noise(double x, double y, double z)
{
	// Find unit cube that contains point
	int X = static_cast<int>(std::floor(x)) & 255;
	int Y = static_cast<int>(std::floor(y)) & 255;
	int Z = static_cast<int>(std::floor(z)) & 255;

	// Find relative x, y, z of point in cube
	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	// Compute fade curves for x, y, z
	double u = math::fade(x);
	double v = math::fade(y);
	double w = math::fade(z);

	// Hash coordinates of the 8 cube corners
	int A = tab[X] + Y;
	int AA = tab[A] + Z;
	int AB = tab[A + 1] + Z;
	int B = tab[X + 1] + Y;
	int BA = tab[B] + Z;
	int BB = tab[B + 1] + Z;

	// Interpolate values
	return	math::lerp(w,
				math::lerp(v,
					math::lerp(u,
						math::grad(tab[AA], x, y, z),
						math::grad(tab[BA], x - 1, y, z)),
					math::lerp(u, math::grad(tab[AB], x, y - 1, z),
						math::grad(tab[BB], x - 1, y - 1, z))),
				math::lerp(v,
					math::lerp(u, math::grad(tab[AA + 1], x, y, z - 1),
						math::grad(tab[BA + 1], x - 1, y, z - 1)),
					math::lerp(u, math::grad(tab[AB + 1], x, y - 1, z - 1),
						math::grad(tab[BB + 1], x - 1, y - 1, z - 1))));
}

float	Noise::Generate2D(math::vec2 pos, float scale, float amplitude, float persistence, int octaves)
{
	float total = 0.0f;
	float frequency = scale;
	float amplitudeFactor = 1.0f;
	float maxAmplitude = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += noise(pos.x * frequency, pos.y * frequency, 0.0) * amplitudeFactor;
		maxAmplitude += amplitudeFactor;
		frequency *= 2.0f;
		amplitudeFactor *= persistence;
	}

	return total / maxAmplitude * amplitude;
}

float	Noise::Generate3D(math::vec3 pos, float scale, float amplitude, float persistence, int octaves)
{
	float total = 0.0f;
	float frequency = scale;
	float amplitudeFactor = 1.0f;
	float maxAmplitude = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += noise(pos.x * frequency, pos.y * frequency, pos.z * frequency) * amplitudeFactor;
		maxAmplitude += amplitudeFactor;
		frequency *= 2.0f;
		amplitudeFactor *= persistence;
	}

	return total / maxAmplitude * amplitude;
}
