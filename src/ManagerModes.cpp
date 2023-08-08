#include <Manager.hpp>
#include <utils.hpp>


int	Manager::changeMode( Clients& client )
{
	std::vector<std::string> split = client.getCmd();
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
	else if	(split[2][1] == 'm')
		return (mOperator(split, _channel, client));
	return 0;
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

bool	Manager::checkChannelParameters(std::string channelName, Clients client, std::vector<std::string> &splits)
{
	if (!checkChannelPassword(channelName, client, splits))
		return false;
	if (!checkChannelLimit(channelName, client))
		return false;
	if (!checkChannelInvite(channelName, client))
		return false;
	return true;
}
