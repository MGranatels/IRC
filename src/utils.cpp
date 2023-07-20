#include <utils.hpp>
#include <Sockets.hpp>

void	ft_bzero(void *s, size_t n)
{
	char *p = (char *)s;
	for (size_t i = 0; i < n; i++)
		p[i] = 0;
}

bool	IsDigit(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}

// Handle Signal Clean program Destroy all clients and close sockets
void	handlesginal(int sig)
{
	(void)sig;
	std::cout << "Signal received" << std::endl;
	exit(0);
}

//takes an element and removes it from the vector
void    removeElementFromVector(std::vector<int> &vector, int element)
{
	std::vector<int>::iterator it = std::remove(vector.begin(), vector.end(), element);
    vector.erase(it, vector.end());
}
