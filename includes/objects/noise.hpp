#ifndef NOISE_HPP
#define NOISE_HPP
#include <vector>
#include "../math/math.hpp"
namespace obj
{
    class Noise {
    public:
        Noise(uint seed, uint size);
        Noise(uint seed);
        ~Noise();
        float Generate2D(math::vec2 pos, int octave, float factor);
        float Generate3D(math::vec3 pos, int octave, float factor);
        // Ajoutez ici les membres et les méthodes de la classe World

    private:
        uint seed;
        uint size = 16;
        std::vector<uint> tab;
    };
}
#endif