#ifndef MESH_HPP
# define MESH_HPP

#include "../math/math.hpp" 

namespace obj
{

class Material
{
	public :
		std::string	name;
		math::vec3	ambientColor;
		math::vec3	diffuseColor;
		math::vec3	specularColor;
		float		specularExponent;
		float		opticalDensity;
		float		dissolve;
		int			illumination;
		std::string	ambientTexture;
		std::string	diffuseTexture;
		std::string	specularTexture;
		std::string	specularHightlight;
		std::string	alphaTexture;
		std::string	bump;

		Material();
		~Material();
};

}

#endif
