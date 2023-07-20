#ifndef	UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <string>
# include <iostream>
# include <cstdlib>

void	ft_bzero(void *s, size_t n);
void	handlesginal(int sig);
bool	IsDigit(std::string str);
void    removeElementFromVector(std::vector<int> &vector, int element);

#endif
