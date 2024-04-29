#include "3DNoise.hpp"

// hashing function
math::vec2	random_gradient(int ix, int iy)
{
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3284157443;

	b ^= a << s | a >> (w - s);
	b *= 1911520717;

	a ^= b << s | b >> (w - s);
	a *= 2048419325;

	float random = a * (3.14159265 / ~(~0u >> 1));
	math::vec2 vec;
	vec.x = cos(random);
	vec.y = sin(random);
	return vec;
}

// Distance Dot product of a position from a grid
float	dot_grid_gradient(int ix, int iy, int x, int y)
{
	math::vec2 gradient = random_gradient(ix, iy);
	math::vec2 distance;
	distance.x = x - ix;
	distance.y = y - iy;
	return distance.dot(gradient);
}

// Cubic interpolation
float interpolation(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}

float	perlin_noise(float x, float y)
{
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - (float)x0;
	float sy = y - (float)y0;

	float n0, n1, ix0, ix1, value;
	n0 = dot_grid_gradient(x0, y0, x, y);
	n1 = dot_grid_gradient(x1, y0, x, y);
	ix0 = interpolation(n0, n1, sx);

	n0 = dot_grid_gradient(x0, y1, x, y);
	n1 = dot_grid_gradient(x1, y1, x, y);
	ix1 = interpolation(n0, n1, sx);

	float res = interpolation(ix0, ix1, sy);
	return res;
}