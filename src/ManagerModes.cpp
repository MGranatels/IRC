#include <Manager.hpp>
#include <utils.hpp>

bool	Manager::checkChannelOp(Channel _channel, int id)
{
	const std::vector <int> opUser = _channel.getOperators();
	for (unsigned int i = 0; i < opUser.size(); i++)
		if (opUser[i] == id)
			return true;
	return false;
}

int	Manager::validateMode(std::vector<std::string> split, Clients client)
{
	// 421	UNKNOWNCOMMAND
	if (isValidChannel(split[1]) != CREATED)
	{
		std::cout << "MEGA DEBUG" << std::endl;
		return (sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :Not in a channel, can't use MODE commands", client.getId()));
	}
	return 1;
}

// i: Set/Remove Invite Only Channel

// t: Set/Remove topic restrictions

// k: Set/Remove channel Key => Password Needed

// o: Give or Take channel Operator Privilege

// l: Set/Remove the user limit channel
