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

bool	Manager::checkFlagFormat(std::string flag)
{
	if (flag.size() != 2)
		return false;
	if (flag[0] != '+' && flag[0] != '-')
		return false;
	return true;
}

int	Manager::validateMode(std::vector<std::string> split, Clients client)
{
	if (isValidChannel(split[1]) != CREATED)
		return (sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :You are not in Any Channel. Please Join a Channel First to use the MODE Command", client.getId()));
	// Lets check if its the channel is being created now, if so We send a message to the channel with the modes
	if (split.size() == 2) {
		Channel& foundChannel = getChannelByName(split[1]);
		BroadcastMessageChan(foundChannel, formatMessage(foundChannel, CHANNELMODEIS) + " +t -i -k +o -l");
		return (0);
	}
	if (split.size() < 3)
		return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Mode. Type for HELP to See a List of Commands", client.getId()));
	if (!checkFlagFormat(split[2]))
		return (sendIrcMessage(formatMessage(client, UMODEUNKNOWNFLAG) + " :Invalid Flag Format. Type for HELP to See a List of Commands", client.getId()));
	if (!checkChannelOp(getChannelByName(split[1]), client.getId()))
		return (sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, can't change modes. Talk to an Admin for Help!", client.getId()));
	return 1;
}
// i: Set/Remove Invite Only Channel

// t: Set/Remove topic restrictions

// k: Set/Remove channel Key => Password Needed

// o: Give or Take channel Operator Privilege

// l: Set/Remove the user limit channel

int	Manager::changeMode(std::vector<std::string> split, Clients client)
{
	Channel& _channel = getChannelByName(split[1]);
	std::map<std::string, ChannelModeStatus> modes = _channel.getModes();
	std::string key(1, split[2][1]);
	std::map<std::string, ChannelModeStatus>::iterator it = modes.find(key);

	std::cout << "Key: " << key << std::endl;
	if (it == modes.end())
		return (sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :Operation does Not exist in the Channel. Type for HELP to See a List of Commands", client.getId()));
	if (split[2][1] == 'k') // Needs password as argument
		kOperator(split, _channel, client);
	else if (split[2][1] == 'o') // Needs client name as argument
		oOperator(split, _channel, client);
	else if (split[2][1] == 'l') // Needs limit number as argument
		lOperator(split, _channel, client);
	else
	{
		if (split[2][0] == '+')
			it->second = MODE_SET;
		else
			it->second = MODE_NOT_SET;
	}
	return 0;
}

int	Manager::kOperator(std::vector<std::string> split, Channel& _channel, Clients& client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", client.getId()));
		_channel.setPassword(split[3]);
		_channel.setMode("k");
	}
	else {
		_channel.setPassword("");
		_channel.unsetMode("k");
	}
	return 1;
}

int	Manager::oOperator(std::vector<std::string> split, Channel& _channel, Clients& client)
{
	if (split.size() != 4)
		return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", client.getId()));
	if (!isValidClient(split[3]))
		return (sendIrcMessage(formatMessage(client, NOSUCHNICK) + " :No such Nickname", client.getId()));
	if (split[2][0] == '+') {
		_channel.addOperator(getClientByNick(split[3]).getId());
		_channel.setMode("o");
	}
	else
	{
		_channel.removeOperator(getClientByNick(split[3]).getId());
		_channel.unsetMode("o");
	}
	return 1;
}

int	Manager::lOperator(std::vector<std::string> split, Channel& _channel, Clients& client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", client.getId()));
		if (!IsDigit(split[3]))
			return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Argument type for Selected Mode. Type HELP For a List of Commands", client.getId()));
		if (std::atoi(split[3].c_str()) <= 0)
			return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Limit Needs to be a Value Higher than 0", client.getId()));
		_channel.setMode("l");
		_channel.setLimit(std::atoi(split[3].c_str()));
	}
	else {
		_channel.unsetMode("l");
		_channel.setLimit(0);
	}
	return 1;
}

