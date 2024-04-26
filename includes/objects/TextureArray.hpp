#ifndef __TEXTUREARRAY_HPP__
# define __TEXTUREARRAY_HPP__

#include <iostream>

namespace obj
{

struct TextureArray
{
	unsigned int		id;
	std::string			type;
	int					width;
	int					height;
	int					nrChannels;
	unsigned long int	depth;
};
	
}

# endif
