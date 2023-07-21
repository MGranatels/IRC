#include <Manager.hpp>
#include <utils.hpp>

void	Manager::checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter)
{
	Clients& foundClient = *iter;
	if (foundClient.getClientSettings() == true)
		return ;
	for (unsigned int i = 0 ; i < splits.size(); i++)
	{
		if (splits[i] == "PASS")
			foundClient.setPassword(splits[i + 1]);
		else if (splits[i] == "NICK")
			foundClient.setNickname(splits[i + 1]);
		else if (splits[i] == "USER")
			foundClient.setUsername(splits[i + 1]);
	}
}


int	Manager::parseActions(std::vector<Clients>::iterator iter, char *buffer, int read)
{
	buffer[read] = 0;
	std::string str(buffer);
	std::vector<std::string> splits = split(str, "\r\n\t ");
	setChanActions();
	if (iter != Manager::getClients().end())
		checkClientData(splits, iter);
	std::string Operation(buffer);
	std::map<std::string, MemberFunctionPointer>::iterator it = _chanActions.find(Operation);
	if (it != _chanActions.end())
	{
        MemberFunctionPointer func = it->second;
       	return func();
    }
	else
        std::cout << Red << "Action not found." << NC << std::endl;
	return(-1);
}
