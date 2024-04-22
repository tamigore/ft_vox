#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include "noise.hpp"
class World {
public:
    World();
    ~World();

    // Ajoutez ici les membres et les méthodes de la classe World

private:
    std::vector<obj::Noise *> noises;
};
#endif