#include <Manager.hpp>
#include <utils.hpp>



bool	Manager::checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter)
{
	Clients& foundClient = *iter;
	if (foundClient.getClientSettings() == true)
		return true;
	for (unsigned int i = 0 ; i < splits.size(); i++)
	{
		if (splits[i] == "PASS")
			foundClient.setPassword(splits[i + 1]);
		else if (splits[i] == "NICK")
			foundClient.setNickname(splits[i + 1]);
		else if (splits[i] == "USER")
			foundClient.setUsername(splits[i + 1]);
	}
	return false;
}


int	Manager::parseActions(std::vector<std::string> splits)
{
	std::cout << splits[0] << std::endl;
	std::map<std::string, MemberFunctionPointer>::iterator it = _chanActions.find(splits[0]);
	if (it != _chanActions.end())
	{
		MemberFunctionPointer func = it->second;
		return func();
    }
	// else
    //     std::cout << Red << "Action not found." << NC << std::endl;
	return(-1);
}
