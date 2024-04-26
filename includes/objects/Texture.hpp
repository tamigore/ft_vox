#ifndef __TEXTURE_HPP__
# define __TEXTURE_HPP__

# include <string>
# include <iostream>

namespace obj
{

struct Texture
{
	unsigned int	id;
	std::string		type;
	int 			width;
	int 			height;
	int 			nrChannels;
};

}

#endif
