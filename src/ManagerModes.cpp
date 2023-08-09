#include <Manager.hpp>
#include <utils.hpp>

void Manager::onMode(std::string event, ModeFunction fun) {
	modeMap.insert(std::pair<std::string, ModeFunction>(event, fun));
}

void Manager::defineModeMap( void )
{
	onMode("k", &Manager::kOperator);
	onMode("o", &Manager::oOperator);
	onMode("l", &Manager::lOperator);
	onMode("i", &Manager::iOperator);
	onMode("t", &Manager::tOperator);
	onMode("m", &Manager::mOperator);
	onMode("b", &Manager::bOperator);
}

void	Manager::changeMode(Clients& client)
{
	Channel& _channel = getChannelByName(client.getCmd()[1]);
	std::string modeName(1, client.getCmd()[2][1]);
	defineModeMap();
	std::map<std::string, ModeFunction>::iterator it = modeMap.find(modeName);
	if (it != modeMap.end())
		(it->second)(_channel, client);
	else
		sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :Operation does Not exist in the Channel. Type for HELP to See a List of Commands", client.getId());
}

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

int	Manager::validateMode(Clients client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (isValidChannel(cmd[1]) != CREATED)
		return (sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " :You are not in Any Channel. Please Join a Channel First to use the MODE Command", client.getId()));
	// Lets check if its the channel is being created now, if so We send a message to the channel with the modes
	if (cmd.size() == 2) {
		Channel& foundChannel = getChannelByName(cmd[1]);
		BroadcastMessageChan(foundChannel, formatMessage(foundChannel, CHANNELMODEIS) + " +t -i -k +o -l");
		return (0);
	}
	if (!checkFlagFormat(cmd[2]))
		return (sendIrcMessage(formatMessage(client, UMODEUNKNOWNFLAG) + " :Invalid Flag Format. Type for HELP to See a List of Commands", client.getId()));
	if (!checkChannelOp(getChannelByName(cmd[1]), client.getId()))
		return (sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, can't change modes. Talk to an Admin for Help!", client.getId()));
	return 1;
}

bool	Manager::checkChannelPassword(std::string channelName, Clients client, std::vector<std::string> &splits)
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
	Channel& channel = getChannelByName(channelName);
	std::map<std::string, ChannelModeStatus> modes = channel.getModes();
	if (modes["i"] == MODE_SET && !channel.isClientInvited(client.getId())) {
		sendIrcMessage(formatMessage(client, INVITEONLYCHAN) + " :Cannot join channel (+i). Access Denied, invitation only", client.getId());
		return false;
	}
	return true;
}

bool	Manager::checkChannelBan(std::string channelName, Clients client)
{
	Channel& channel = getChannelByName(channelName);
	if (channel.isClientBanned(client.getId())) {
		sendIrcMessage(formatMessage(client, BANNEDFROMCHAN) + " :Cannot join channel (+b). Access Denied, you are banned", client.getId());
		return false;
	}
	return true;
}

bool	Manager::checkChannelParameters(std::string channelName, Clients client, std::vector<std::string> &splits)
{
	if (!checkChannelBan(channelName, client))
		return false;
	if (!checkChannelPassword(channelName, client, splits))
		return false;
	if (!checkChannelLimit(channelName, client))
		return false;
	if (!checkChannelInvite(channelName, client))
		return false;
	return true;
}
