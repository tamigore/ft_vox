#include "Noise.hpp"

#include <numeric>
#include <random>
#include <algorithm>

using namespace obj;

Noise::Noise() {}

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

Noise::~Noise() {}

double	Noise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double	Noise::lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double	Noise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
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
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = tab[X] + Y;
	int AA = tab[A] + Z;
	int AB = tab[A + 1] + Z;
	int B = tab[X + 1] + Y;
	int BA = tab[B] + Z;
	int BB = tab[B + 1] + Z;

	// Interpolate values
	return	lerp(w,
				lerp(v,
					lerp(u,
						grad(tab[AA], x, y, z),
						grad(tab[BA], x - 1, y, z)),
					lerp(u, grad(tab[AB], x, y - 1, z),
						grad(tab[BB], x - 1, y - 1, z))),
				lerp(v,
					lerp(u, grad(tab[AA + 1], x, y, z - 1),
						grad(tab[BA + 1], x - 1, y, z - 1)),
					lerp(u, grad(tab[AB + 1], x, y - 1, z - 1),
						grad(tab[BB + 1], x - 1, y - 1, z - 1))));
}
