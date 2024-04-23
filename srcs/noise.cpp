#include "../includes/noise.hpp"

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

float interpolate(float a0, float a1, float w)
{
    // printf("a0: %f, a1: %f, w: %f\n", a0, a1, w);
    return (a1 - a0) * w + a0;
}

math::vec2 randomGradient(int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443; 
    b ^= a << s | a >> (w-s);
    b *= 1911520717; 
    a ^= b << s | b >> (w-s);
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    math::vec2 v;
    v.x = cos(random); v.y = sin(random);
    // std::cout << v << std::endl;
    return v;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
    // Get gradient from integer coordinates
    math::vec2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*gradient.x + dy*gradient.y);
}

float	perlin(float x, float y)
{
    // Determine grid cell coordinates
    // printf("x: %f, y: %f\n", x, y);
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;
    // printf("x0: %d, x1: %d, y0: %d, y1: %d\n", x0, x1, y0, y1);
    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;
    // printf("sx: %f, sy: %f\n", sx, sy);
    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    // printf("n0: %f, n1: %f, ix0: %f\n", n0, n1, ix0);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

float	Noise::Generate2D(math::vec2 pos, int octave, float factor)
{
    float value = 0;
    float frequency = 1;
    float amplitude = 1;

    for (int i = 0; i < octave; i++)
    {
        value += perlin(pos.x * frequency, pos.y * frequency) * amplitude;
        // printf("perlin: %f\n", perlin(pos.x * frequency, pos.y * frequency) * amplitude);
        frequency *= 2;
        amplitude *= factor;
    }
    value = (value + 1) / 2;
    // printf("value: %f, posx :%f, posy :%f\n", value, pos.x, pos.y);
    return value;
}
