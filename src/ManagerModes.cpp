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

int	Manager::changeMode(std::vector<std::string> split, Clients client)
{
	Channel& _channel = getChannelByName(split[1]);
	std::map<std::string, ChannelModeStatus> modes = _channel.getModes();
	std::string key(1, split[2][1]);
	std::map<std::string, ChannelModeStatus>::iterator it = modes.find(key);

	if (it == modes.end())
		return (sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :Operation does Not exist in the Channel. Type for HELP to See a List of Commands", client.getId()));
	if (split[2][1] == 'k') // Needs password as argument
		return (kOperator(split, _channel, client));
	else if (split[2][1] == 'o') // Needs client name as argument
		return (oOperator(split, _channel, client));
	else if (split[2][1] == 'l') // Needs limit number as argument
		return (lOperator(split, _channel, client));
	else if (split[2][1] == 'i')
		return (iOperator(split, _channel));
	else if (split[2][1] == 't')
		return (tOperator(split, _channel, client));
	return 0;
}

int	Manager::tOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		_channel.setMode("t");
		_channel.setTopic(split[3]);
		sendIrcMessage(formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), _client.getId());
	}
	else {
		_channel.unsetMode("t");
		_channel.setTopic("");
		sendIrcMessage(formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set", _client.getId());
	}
	return 1;
}

int	Manager::iOperator(std::vector<std::string> split, Channel& _channel)
{
	if (split[2][0] == '+')
		_channel.setMode("i");
	else
		_channel.unsetMode("i");
	return 1;
}

int	Manager::kOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		_channel.setPassword(split[3]);
		_channel.setMode("k");
	}
	else {
		_channel.setPassword("");
		_channel.unsetMode("k");
	}
	return 1;
}

int	Manager::oOperator(std::vector<std::string> split, Channel& channel, Clients& _client)
{
	if (_client.getNickname() == split[3])
		return (sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " :Permission denied, can't remove your own privileges.", _client.getId()));
	if (split.size() != 4)
		return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
	if (!isValidClient(split[3]))
		return (sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId()));
	if (split[2][0] == '+') {
		channel.addOperator(getClientByNick(split[3]).getId());
		channel.setMode("o");
	}
	else
	{
		channel.removeOperator(getClientByNick(split[3]).getId());
		channel.unsetMode("o");
	}
	BroadcastMessageChan(channel, formatMessage(_client, NAMREPLY) + " = " + channel.getName() + " :" + getUsersList(channel));
	BroadcastMessageChan(channel, formatMessage(_client, ENDOFNAMES) + " " + channel.getName() + " :End of NAMES list");
	return 1;
}

int	Manager::lOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		if (!IsDigit(split[3]))
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Argument type for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		if (std::atoi(split[3].c_str()) <= 0)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Limit Needs to be a Value Higher than 0", _client.getId()));
		_channel.setMode("l");
		_channel.setLimit(std::atoi(split[3].c_str()));
	}
	else {
		_channel.unsetMode("l");
		_channel.setLimit(0);
	}
	return 1;
}


bool	Manager::checkChannelPassword(std::string channelName, Clients client, std::vector<std::string> splits)
{
	Channel& _channel = getChannelByName(channelName);
	std::map<std::string, ChannelModeStatus> modes = _channel.getModes();
	if (modes["k"] == MODE_SET)
	{
		if (splits.size() != 3 || splits[2].empty() || _channel.getPassword() != splits[2]) {
			sendIrcMessage(formatMessage(client, BADCHANNELKEY) + " :Cannot join channel (+k), bad Password", client.getId());
			return false;
		}
	}
	return true;
}

bool	Manager::checkChannelLimit(std::string channelName, Clients client)
{
	Channel& _channel = getChannelByName(channelName);
	std::map<std::string, ChannelModeStatus> modes = _channel.getModes();

	if (modes["l"] == MODE_SET && _channel.getClients().size() >= _channel.getLimit())
	{
		sendIrcMessage(formatMessage(client, CHANNELISFULL) + " :Cannot join channel (+l), Channel is Full", client.getId());
		return false;
	}
	return true;
}

bool	Manager::checkChannelInvite(std::string channelName, Clients client)
{
	Channel& _channel = getChannelByName(channelName);
	std::map<std::string, ChannelModeStatus> modes = _channel.getModes();
	if (modes["i"] == MODE_SET) {
		sendIrcMessage(formatMessage(client, INVITEONLYCHAN) + " :Cannot join channel (+i). Access Denied, invitation only", client.getId());
		return false;
	}
	return true;
}

bool	Manager::checkChannelParameters(std::string channelName, Clients client, std::vector<std::string> splits)
{
	if (!checkChannelPassword(channelName, client, splits))
		return false;
	if (!checkChannelLimit(channelName, client))
		return false;
	if (!checkChannelInvite(channelName, client))
		return false;
	return true;
}
