#ifndef __VERTEX_HPP__
# define __VERTEX_HPP__

#include "../math/vec3.hpp"
#include "../math/vec2.hpp"

// #define MAX_BONE_INFLUENCE 10

namespace obj
{

struct vertex
{
	math::vec3 Position;
	math::vec3 Normal;
	math::vec2 Texture;
	math::vec3 Tangent;
	math::vec3 Bitangent;

	float	face;
    float	block;

	// int m_BoneIDs[MAX_BONE_INFLUENCE];
	// float m_Weights[MAX_BONE_INFLUENCE];
};

}

#endif
