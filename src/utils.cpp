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

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::size_t start = 0;
    std::size_t end = str.find_first_of(delimiter);

    while (end != std::string::npos) {
        std::string token = str.substr(start, end - start);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        start = end + 1;
        end = str.find_first_of(delimiter, start);
    }

    std::string lastToken = str.substr(start);
    if (!lastToken.empty()) {
        tokens.push_back(lastToken);
    }
    return tokens;
}

void	removeWhitespace(std::string& str)
{
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}
