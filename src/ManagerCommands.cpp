#include <Manager.hpp>
#include <utils.hpp>

void	Manager::checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter)
{
	static	int	status;

	Clients& foundClient = *iter;
	if (foundClient.getClientSettings() == true)
		return ;
	for (unsigned int i = 0 ; i < splits.size(); i++)
	{
		if (splits[i] == "PASS" && status++)
			foundClient.setPassword(splits[i + 1]);
		else if (splits[i] == "NICK" && status++)
			foundClient.setNickname(splits[i + 1]);
		else if (splits[i] == "USER" && status++)
			foundClient.setUsername(splits[i + 1]);
	}
	if (status == 3)
	{
		foundClient.setClientSettings(true);
		status = 0;
	}
}


void	Manager::parseCommands(std::vector<Clients>::iterator iter, char *buffer, int read)
{
	buffer[read] = 0;
	std::string str(buffer);
	std::vector<std::string> splits = split(str, "\r\n\t ");
	if (iter != Manager::getClients().end())
		checkClientData(splits, iter);
	// Use the cmd vector to parse the commands
	// Vector of strings that is in the client class

	// CALADO: Pesquisa sobre um tipo de container que se chama std::map. 
	// E um tipo de variavel que emparalhada variaveis com funcoes.
	// Ou variaveis com outras variaveis e top para fazer parsings

	// std::vector<std::string>::iterator it = _chanoper.begin();
	// for (; it != _chanoper.end(); ++it)
	// 	if (*it == str)
	// 	{
	// 		std::cout << *it << "was identified." << std::endl;
	// 		break;
	// 	}
	// if (it == _chanoper.end())
	// 	std::cout << "no channel operator identified" << std::endl;
}
