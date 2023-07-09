#include <utils.hpp>
#include <Clients.hpp>

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
	for (unsigned int i = 0; i < Clients::_clients.size(); i++)
	{
		if (Clients::_clients[i] != NULL)
			delete Clients::_clients[i];
	}
	exit(0);
}