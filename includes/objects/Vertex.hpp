#ifndef __VERTEX_HPP__
# define __VERTEX_HPP__

#include "../math/vec3.hpp"
#include "../math/vec2.hpp"

namespace obj
{

struct Vertex
{
	math::vec3	Position;
	math::vec3	Normal;
	math::vec2	TextureCoordonates;
	int			Face;
	int			Block;
};

}

#endif
