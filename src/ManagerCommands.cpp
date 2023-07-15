#include <Manager.hpp>



void	Manager::parseCommands(std::vector<Clients>::iterator iter, char *buffer, int read)
{
	(void)iter;
	(void)read;
	std::string str = buffer;

	// Use the cmd vector to parse the commands
	// Vector of strings that is in the client class

	std::vector<std::string>::iterator it = _chanoper.begin();
	for (; it != _chanoper.end(); ++it)
		if (*it == str)
		{
			std::cout << *it << "was identified." << std::endl;
			break;
		}
	if (it == _chanoper.end())
		std::cout << "no channel operator identified" << std::endl;
}