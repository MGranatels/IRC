#include <Manager.hpp>
#include <Channel.hpp>

std::vector<Clients> Manager::_clients;
std::vector<Channel> Manager::_channels;
std::string Manager::hostname = "localhost";
std::map<std::string, ActionFunction> Manager::actionMap;

void Manager::on(std::string event, ActionFunction fun) {
	actionMap.insert(std::pair<std::string, ActionFunction>(event, fun));
}

void Manager::defineActionMap( )
{
	on("JOIN", &Manager::joinAction);
	on("QUIT", &Manager::quitAction);
	on("PART", &Manager::partAction);
	on("MODE", &Manager::modeAction);
	on("TOPIC", &Manager::topicAction);
	on("JOIN", &Manager::joinAction);
	on("INVITE", &Manager::inviteAction);
	on("PRIVMSG", &Manager::privAction);
	//on("NICK", &Manager::nickAction);
    // actionMap["KICK"] = &Manager::kickAction;
}

bool	Manager::checkClientData(std::vector<std::string> &splits, std::vector<Clients>::iterator iter)
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

int	Manager::runChanActions(std::vector<std::string>& splits, int clientId, std::string full_message) {
	std::string actionName = splits[0];
	for (unsigned int i = 0; i < splits.size(); i++)
		std::cout << i << " Split: " <<  splits[i] << std::endl;
	defineActionMap();
	if (!isValidClient(clientId))
		return -1;
	// Find the action function in the map
	std::map<std::string, ActionFunction>::iterator it = actionMap.find(actionName);
	if (it != actionMap.end())
		(it->second)(*getClientById(clientId), splits, full_message);
	return -1;
}

void	Manager::partAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage )
{
	(void) fullMessage;
	std::string partMessage = "";

	if (splits.size() > 2)
		partMessage = splits[2];
	// Find the channel
	Channel& channel = Manager::getChannelByName(splits[1]);
	// Check if the client is in the channel
	if (!channel.isClientInChannel(client.getId()))
		return ;
	// Notify other clients in the channel about the PART
	if (!partMessage.empty())
		BroadcastMessageChan(channel, formatMessage(client, "PART_CHANNEL") + " " + channel.getName() + " " + partMessage);
	else
		BroadcastMessageChan(channel, formatMessage(client, "PART_CHANNEL") + " " + channel.getName());
	// Send a PART message to the client to indicate they left the channel.
	sendIrcMessage(formatMessage(client) + " PART " + channel.getName(), client.getId());
	BroadcastMessageChan(channel, formatMessage(client, "QUIT_CHANNEL") + " :has quit");
	// Remove the user from the channel
	channel.removeClient(client.getId());
	messageUpdateUserList(channel, client);
	return ;
}

void	Manager::joinAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage )
{
	(void) fullMessage;
	// First, check if the channel exists
	std::string &channelName = splits[1];
	int isValidChan = isValidChannel(channelName);

	std::cout << "Check Nick in Client Vector " << client.getNickname() << std::endl ;
	if (isValidChan == CREATED)
	{
		if (!checkChannelParameters(channelName, client, splits))
			return ;
		Channel& existingChannel = getChannelByName(channelName);
		existingChannel.addClient(client.getId());
		joinProtocol(client, existingChannel, client.getId());
	}
	else if (isValidChan == VALID_NAME)
	{
		_channels.push_back(Channel(channelName));
		_channels.back().addClient(client.getId());
		_channels.back().addOperator(client.getId());
		joinProtocol(client, _channels.back(), client.getId());
	}
	else if (isValidChan == NOT_VALID)
		sendIrcMessage(formatMessage(client, BADCHANNELNAME) + " " + channelName + " :Invalid channel name, try with #", client.getId());
	return ;
}

void	Manager::quitAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage )
{
	(void) fullMessage;
	(void) splits;
	removeClientFromAllChannels(client.getId());
	sendIrcMessage(formatMessage(client) + " QUIT :Goodbye!", client.getId());
	removeClient(client.getId());
	return ;
}

void	Manager::privAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage)
{
	//TODO: remember later to Verify User Permissions

	std::string &recipient = splits[1];
	std::vector<std::string> message = split(fullMessage, ":");

	if (isValidChannel(recipient) == CREATED)
	{
		Channel& channel = getChannelByName(recipient);
		std::cout  << channel.isClientMuted(client.getId()) << std::endl;
		if (channel.isClientMuted(client.getId()))
		{
			sendIrcMessage(formatMessage(client, CANNOTSENDTOCHAN) + " " + recipient + " :Cannot send message to channel, you have been Muted, shiuuuuuuu!", client.getId());
			return ;
		}
		BroadcastMessageChan(client.getId(), channel, formatMessage(client) + " PRIVMSG " + recipient + " " + message[1]);
	}
	else if (isValidClient(recipient))
	{
		int recipientId = getClientByNick(recipient).getId();
		sendIrcMessage(formatMessage(client) + " PRIVMSG " + recipient + " " + message[1], recipientId);
	}
}

void	Manager::kickAction( void )
{
	std::cout << "Tas todo ze queres kickar quem crl" << std::endl;
	//	KICK <channel> <user> :<reason>
	return ;
}

void	Manager::modeAction( Clients& client, std::vector<std::string>& splits, std::string fullMessage )
{
	(void) fullMessage;

	if (!validateMode(splits, client))
		return ;
	changeMode(splits, client);
	return ;
}

void	Manager::topicAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage )
{
	(void) fullMessage;
	Channel& _channel = getChannelByName(splits[1]);

	if (splits.size() < 3 && _channel.getTopic().empty()) {
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set", client.getId());
		return ;
	}
	if (splits.size() < 3)
	{
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), client.getId());
		return ;
	}
	if (!_channel.isClientOperator(client.getId())) {
		sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, you're not channel operator.", client.getId());
		return ;
	}
	//Check if user has permissions in channel
	if (!_channel.isModeSet("t"))
	{
		sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, topic Channel 't' not set.", client.getId());
		return ;
	}
	_channel.setTopic(splits[2]);
	//sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), client.getId());
	BroadcastMessageChan(_channel, formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic());
}

void	Manager::inviteAction( Clients& inviter, std::vector<std::string>& splits, std::string fullMessage )
{
	(void) fullMessage;

	std::string &channelName = splits[2];
	std::string	&invitedClient = splits[1];
	int isValidChan = isValidChannel(channelName);
	if (isValidChan == CREATED)
	{
		Channel &channel = getChannelByName(channelName);
		if (!channel.isClientOperator(inviter.getId()))
		{
			sendIrcMessage(formatMessage(inviter, CHANOPRIVSNEEDED) + " " + channelName + " :Permission denied, you're not channel operator.", inviter.getId());
			return ;
		}
		if (!channel.isClientInChannel(inviter.getId()))
		{
			sendIrcMessage(formatMessage(inviter, ERR_USERONCHANNEL) + " " + channelName + " :User not in Channel", inviter.getId());
			return ;
		}
		if (!isValidClient(invitedClient))
		{
			sendIrcMessage(formatMessage(inviter, NOSUCHNICK) + " " + invitedClient + " :Not such user in Server", inviter.getId());
			return ;
		}
		if (channel.isClientInChannel(getClientByNick(invitedClient).getId()))
		{
			sendIrcMessage(formatMessage(inviter, ERR_USERONCHANNEL) + " " + invitedClient + " :user already in Channel " + channelName, inviter.getId());
			return ;
		}
		// :<server> 341 <invited_user> <channel> <inviter>
		channel.addInvitee(getClientByNick(invitedClient).getId());
		sendIrcMessage(formatMessage(inviter, INVITING) + " " + invitedClient + " " + channelName, inviter.getId());
		sendIrcMessage(formatMessage(inviter) + " NOTICE " + invitedClient + " you have been invited to join " + channelName, getClientByNick(invitedClient).getId());
	}
	else
		sendIrcMessage(formatMessage(inviter, ERR_NOSUCHCHANNEL) + " " + channelName + " :Not such channel in Server ",  inviter.getId());
}

void	Manager::nickAction( Clients& client, std::vector<std::string>& splits, std::string fullMessage )
{
	(void) fullMessage;

	if (splits.size() < 2)
	{
		sendIrcMessage(formatMessage(client, NONICKNAMEGIVEN) + " :No nickname given", client.getId());
		return ;
	}
	if (splits[1].size() > 9)
	{
		sendIrcMessage(formatMessage(client, ERRONEUSNICKNAME) + " " + splits[1] + " :Erroneous nickname", client.getId());
		return ;
	}
	if (isValidClient(splits[1]))
	{
		sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + splits[1] + " :Nickname is already in use", client.getId());
		return ;
	}
	client.setNickname(splits[1]);
	sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + client.getNickname() + splits[1] + " :Nickname changed successfully", client.getId());
}

/*int	Manager::muteAction( const Clients &client, std::vector<std::string> &splits)
{
	if (splits.size() < 3)
		return (sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Action. Type HELP For a List of Commands", client.getId()));
	std::string &channelName = splits[2];
	if (!isValidClient(splits[1]))
		return (sendIrcMessage(formatMessage(client, NOSUCHNICK) + " " + splits[1] + " :No such user in Server", client.getId()));
	Clients	&mutedClient = getClientByNick(splits[1]);
	int isValidChan = isValidChannel(channelName);
	if (isValidChan == CREATED)
	{
		Channel &channel = getChannelByName(channelName);
		if (!channel.isClientOperator(client.getId()))
			return (sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + channelName + " :Permission denied, you're not channel operator.", client.getId()));
		if (!channel.isClientInChannel(mutedClient.getId()))
			return (sendIrcMessage(formatMessage(client, ERR_USERONCHANNEL) + " " + channelName + " :User not in Channel", client.getId()));
		if (channel.isClientMuted(mutedClient.getId()))
			return (sendIrcMessage(formatMessage(client, ERR_USERONCHANNEL) + " " + mutedClient.getNickname() + " :user already muted" + channelName, client.getId()));
		if (splits[0] == "MUTED")
			channel.addMuted(mutedClient.getId());
		else
			channel.removeMuted(mutedClient.getId());
	}
	else
		return (sendIrcMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :Not such channel in Server ", client.getId()));
	return 1;
}*/
