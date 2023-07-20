#ifndef	UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <string>
# include <iostream>
# include <cstdlib>
# include <vector>

void						ft_bzero(void *s, size_t n);
void						handlesginal(int sig);
void						removeWhitespace(std::string& str);
bool						IsDigit(std::string str);
std::vector<std::string> 	split(const std::string& str, const std::string& delimiter);

#endif