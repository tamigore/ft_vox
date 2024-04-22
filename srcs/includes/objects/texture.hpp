#ifndef __TEXTURE_HPP__
# define __TEXTURE_HPP__

# include <string>

namespace obj
{

struct texture
{
    unsigned int	id;
    std::string		type;
    std::string		path;
};

}

#endif
