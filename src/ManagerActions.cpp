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
	if (splits[0].compare("JOIN") == 0)
		return( Manager::joinAction(splits[1], clientId) );
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
		return( Manager::topicAction() );
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

int	Manager::joinAction( std::string channelName, int clientId )
{
	// First, check if the channel exists
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	std::cout << clientId << std::endl;
	Clients& client = *iter;
	std::cout << "Check Nick in Client Vector " << client.getNickname() << std::endl ;
	if (isValidChannel(channelName) == CREATED)
	{
		Channel& existingChannel = getChannelByName(channelName);
		existingChannel.addClient(clientId);
		joinProtocol(client, existingChannel, clientId);
	}
	else if (isValidChannel(channelName) == VALID_NAME)
	{
		_channels.push_back(Channel(channelName));
		_channels.back().addClient(clientId);
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
	if (isValidChannel(splits[1]) == CREATED)
	{
		//:user1!user1@localhost PRIVMSG #test :Hello, everyone!\r\n
		BroadcastMessageChan(getChannelByName(splits[1]), formatMessage(client) + " PRIVMSG #" + splits[1] + " " + splits[2]);
	}
	else if (isValidClient(splits[1]))
	{
		std::cout << "it's for a friend" << std::endl;
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
	// 472 ERR_UNKNOWNMODE => When a user try's to change a channel mode that does not exist
	// 501 ERR_UMODEUNKNOWNFLAG => When user try's to set or unset a existing channel mode but with the wrong flag
	// 461 ERR_NEEDMOREPARAMS => When user try's to change a channel mode and does not add a flag to it (few arguments per say)
	// 482 ERR_CHANOPRIVSNEEDED  When user does not have admin permission to change modes.
	std::cout << LightCyan << "Channel Modes Debugger" << std::endl << NC;
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	std::cout << clientId << std::endl;
	Clients& client = *iter;
	for (unsigned int i = 0; i < split.size(); i++)
		std::cout << i << " Split: " <<  split[i] << std::endl;
	validateMode(split, client);
	return(1);
}

int	Manager::topicAction( void )
{
	std::cout << "A Gabi nao pode ouvir este topico" << std::endl;
	return(1);
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
