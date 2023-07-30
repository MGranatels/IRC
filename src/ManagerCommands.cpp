#include <Manager.hpp>
#include <utils.hpp>

bool	Manager::checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter, int id)
{
	Clients& foundClient = *iter;
	if (foundClient.getClientSettings() == true)
		return true;
	for (unsigned int i = 0 ; i < splits.size(); i++)
	{
		if (splits[i] == "PASS")
			foundClient.setPassword(splits[i + 1]);
		else if (splits[i] == "NICK")
			foundClient.setNickname(splits[i + 1], id);
		else if (splits[i] == "USER")
			foundClient.setUsername(splits[i + 1]);
	}
	return false;
}
