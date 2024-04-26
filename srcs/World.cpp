#include "World.hpp"

World::World(): skybox_active(true), renderDistance(10)
{
	skybox.setup();
	skybox.load();
}

World::~World()
{
}
