#include <utils.hpp>

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