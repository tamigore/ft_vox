#ifndef __UTILS_HPP__
# define __UtILS_HPP__

# include <string>
# include <vector>

std::string	firstToken(const std::string &in);
std::string	tail(const std::string &in);
void 		split(const std::string &in, std::vector<std::string> &out, std::string token);

#endif
