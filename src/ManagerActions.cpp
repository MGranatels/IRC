#include <Manager.hpp>
#include <Channel.hpp>

std::vector<Clients> Manager::_clients;
std::vector<Channel> Manager::_channels;
std::string Manager::hostname = "localhost";

// aqui podes passar mais parametros

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

int	Manager::runChanActions( std::vector<std::string> splits, int clientId)
{
	for (unsigned int i = 0; i < splits.size(); i++)
		std::cout << i << " Split: " <<  splits[i] << std::endl;
	if (splits[0].compare("JOIN") == 0)
		return( Manager::joinAction(splits[1], clientId, splits) );
	//else if (splits[0].compare("KICK") == 0)
		//return( Manager::kickAction() );
	else if (splits[0].compare("QUIT") == 0)
		return(Manager::quitAction(clientId));
	else if (splits[0].compare("PART") == 0)
		if (splits.size() > 2)
			return(partAction(splits[1], clientId, splits[2]));
		else
			return(partAction(splits[1], clientId, ""));
	else if (splits[0].compare("MODE") == 0)
		return( Manager::modeAction(splits, clientId) );
	else if (splits[0].compare("TOPIC") == 0)
		return( Manager::topicAction(*getClientById(clientId), splits));
	else if (splits[0].compare("INVITE") == 0)
		return( Manager::inviteAction(splits[1], clientId) );
	else if (splits[0].compare("PRIVMSG") == 0)
		return( Manager::privAction( *getClientById(clientId), splits) );
	else if (splits[0].compare("NICK") == 0)
		return(0);
	return (-1);
}

int	Manager::partAction(std::string channelName, int clientId, std::string partMessage)
{
	// Find the client who wants to part
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	if (iter == _clients.end()) {
		// Client not found
		return -1;
	}
	Clients& partingClient = *iter;
	// Find the channel
	Channel& channel = Manager::getChannelByName(channelName);
	// Check if the client is in the channel
	if (!channel.isClientInChannel(clientId)) {
		// Client is not in the channel, nothing to do
		return -1;
	}
	// Notify other clients in the channel about the PART
	if (!partMessage.empty())
		BroadcastMessageChan(channel, formatMessage(partingClient, "PART_CHANNEL") + " " + channel.getName() + " " + partMessage);
	else
		BroadcastMessageChan(channel, formatMessage(partingClient, "PART_CHANNEL") + " " + channel.getName());
	// Send a PART message to the client to indicate they left the channel.
	sendIrcMessage(formatMessage(partingClient) + " PART " + channel.getName(), clientId);
	BroadcastMessageChan(channel, formatMessage(partingClient, "QUIT_CHANNEL") + " :has quit");
	channel.removeClient(clientId);
	BroadcastMessageChan(channel, formatMessage(partingClient, "353") + " = " + channel.getName() + " :" + getUsersList(channel));
	BroadcastMessageChan(channel, formatMessage(partingClient, "366") + " " + channel.getName() + " :End of NAMES list");
	// Remove the user from the channel
	return 1;
}

int	Manager::joinAction( std::string channelName, int clientId, std::vector<std::string> splits )
{
	// First, check if the channel exists
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	std::cout << clientId << std::endl;
	Clients& client = *iter;

	std::cout << "Check Nick in Client Vector " << client.getNickname() << std::endl ;
	if (isValidChannel(channelName) == CREATED)
	{
		if (!checkChannelParameters(channelName, client, splits))
			return 0;
		Channel& existingChannel = getChannelByName(channelName);
		existingChannel.addClient(clientId);
		joinProtocol(client, existingChannel, clientId);
	}
	else if (isValidChannel(channelName) == VALID_NAME)
	{
		_channels.push_back(Channel(channelName));
		_channels.back().addClient(clientId);
		_channels.back().addOperator(clientId);
		joinProtocol(client, _channels.back(), clientId);
	}
	return 1;
}

int	Manager::quitAction(int clientId)
{
	// Find the client who wants to quit
	std::vector<Clients>::iterator iter = getClientById(clientId);
	if (iter == _clients.end()) {
		// Client not found
		return -1;
	}
	Clients& quittingClient = *iter;

	// Remove the client from all channels
	removeClientFromAllChannels(clientId);
	// Notify the quitting client that they have quit
	sendIrcMessage(formatMessage(quittingClient) + " QUIT :Goodbye!", clientId);
	// Remove client from clients list
	return 1;
}


int	Manager::privAction( const Clients &client, std::vector<std::string> splits)
{
	//TODO: remember later to Verify User Permissions

	std::string &recipient = splits[1];
	std::string &message = splits[2];

	for (size_t i = 3; i < splits.size(); i++)
		message += " " + splits[i];

	if (isValidChannel(recipient) == CREATED)
	{
		//:user1!user1@localhost PRIVMSG #test :Hello, everyone!\r\n
		BroadcastMessageChan(client.getId(), getChannelByName(recipient), formatMessage(client) + " PRIVMSG " + recipient + " " + message);
	}
	else if (isValidClient(recipient))
	{
		int recipientId = getClientByNick(recipient).getId();
		sendIrcMessage(formatMessage(client) + " PRIVMSG " + recipient + " " + message, recipientId);
	}
	return (1);
}

int	Manager::kickAction( void )
{
	std::cout << "Tas todo ze queres kickar quem crl" << std::endl;
	//	KICK <channel> <user> :<reason>
	return(1);
}

int	Manager::modeAction( std::vector<std::string> split, int clientId )
{
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	std::cout << clientId << std::endl;
	Clients& client = *iter;
	if (!validateMode(split, client))
		return (1);
	changeMode(split, client);
	return(1);
}

int	Manager::topicAction( Clients &client, std::vector<std::string> splits )
{
	Channel& _channel = getChannelByName(splits[1]);

	if (splits.size() < 3 && _channel.getTopic().empty())
		return (sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set", client.getId()));
	if (splits.size() < 3)
		return (sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), client.getId()));
	//Check if user has permissions in channel
	if (!_channel.isClientOperator(client.getId()))
		return (sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, you're not channel operator.", client.getId()));
	if (!_channel.isModeSet("t"))
		return (sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, topic Channel 't' not set.", client.getId()));
	_channel.setTopic(splits[2]);
	return(sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), client.getId()));
}

int	Manager::inviteAction( std::string nickName, int clientId )
{
	(void)nickName;
	(void)clientId;
	// RPL_INVITING 341 to invite user
	// ERR_NOTONCHANNEL 442 when user not in channel
	// ERR_CHANOPRIVSNEEDED 482 When user does not have admin permission

	// Check which channel the user is sending the message from, how can I do that?

	// If user is not in any channel send an error response

	// If nickname does not exist send error response

	// If user does not have permissions to invite send error response 482


	std::cout << "tao, pega umas cervejinhas e buga" << std::endl;
	return(1);
}
