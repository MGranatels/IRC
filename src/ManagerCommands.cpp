#include <Manager.hpp>
#include <utils.hpp>

void	Manager::checkCommands(std::vector<std::string> splits, std::vector<Clients>::iterator iter)
{
	Clients& foundClient = *iter;

	for (unsigned int i = 0 ; i < splits.size(); i++)
	{
		std::cout << i << ": Print Splits: ";
		std::cout << splits[i] << std::endl;
	}
	foundClient.setNickname(splits[4]);
	foundClient.setUsername(splits[5]);
	foundClient.setPassword(splits[5]);
	(void)foundClient;
}


void	Manager::parseCommands(std::vector<Clients>::iterator iter, char *buffer, int read)
{
	(void)iter;
	(void)read;
	buffer[read] = 0;
	std::string str(buffer);
	std::vector<std::string> splits = split(str, ' ');
	checkCommands(splits, iter);
	// Use the cmd vector to parse the commands
	// Vector of strings that is in the client class

	// CALADO: Pesquisa sobre um tipo de container que se chama std::map. 
	// E um tipo de variavel que emparalhada variaveis com funcoes.
	// Ou variaveis com outras variaveis e top para fazer parsings
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
