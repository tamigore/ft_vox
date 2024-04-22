#ifndef __FACE_HPP__
# define __FACE_HPP__

#include <vector>

#include "../../includes/math/math.hpp"

namespace obj
{

struct face
{
	// public:
		std::vector<unsigned int>	m_vertice_index;
		std::vector<unsigned int>	m_normal_index;
		std::vector<unsigned int>	m_texture_index;

		// face();
		// ~face();
		// std::vector<unsigned int> triangulate(std::vector<math::vec3> vertices);
};

}

#endif