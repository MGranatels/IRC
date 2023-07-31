#include <Manager.hpp>
#include <Channel.hpp>

std::vector<Clients> Manager::_clients;
std::vector<Channel> Manager::_channels;
std::string Manager::_hostname = "localhost";

// aqui podes passar mais parametros

int	Manager::runChanActions( std::vector<std::string> splits, int clientId)
{
	if (splits[0].compare("JOIN") == 0)
		return( Manager::joinAction(splits[1], clientId) );
	else if (splits[0].compare("KICK") == 0)
		return( Manager::kickAction() );
	else if (splits[0].compare("MODE") == 0)
		return( Manager::modeAction() );
	else if (splits[0].compare("TOPIC") == 0)
		return( Manager::topicAction() );
	else if (splits[0].compare("INVITE") == 0)
		return( Manager::inviteAction() );
	else if (splits[0].compare("PRIVMSG") == 0)
		return( Manager::privAction() );
	else if (splits[0].compare("NICK") == 0)
		return(0);
	return (-1);
}

const std::string formatMessage(Clients &client)
{
	return (":" + client.getUserNickname() + "!" + client.getUsername() + "@" + Manager::_hostname);
}

// aqui podes passar mais parametros

int	Manager::joinAction( std::string channelName, int clientId )
{
	// First, check if the channel exists
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	std::cout << clientId << std::endl;
	Clients& client = *iter;
	std::cout << "Check Nick in Client Vector " << client.getNickname() << std::endl ;
	if (isValidChannel(channelName) == VALID_NAME)
	{
		// Channel doesn't exist, so create it
		_channels.push_back(Channel(channelName));
		_channels.back().addUser(clientId);
		// Send the JOIN message to the client
		sendIrcMessage(formatMessage(client) + " JOIN " + channelName, clientId);
		//:<server_hostname> 332 <user_nickname> <channel_name> :<channel_topic>
		sendIrcMessage(":" + _hostname + " 332 " + client.getNickname() + " " + channelName + " :No topic is set", clientId);
		sendIrcMessage(":" + _hostname + " 353 " + client.getNickname() + " = " + channelName + " :" + getUsersList(_channels.back()), clientId);
		sendIrcMessage(":" + _hostname + " 366 " + client.getNickname() + " " + channelName + " :End of NAMES list", clientId);
	}
	else if (isValidChannel(channelName) == CREATED)
	{
		Channel& existingChannel = getChannelByName(channelName);
		// Add the client to the user list of the existing channel
		existingChannel.addUser(clientId);
		// Send the JOIN message to the client
		sendIrcMessage(formatMessage(client) + " JOIN " + channelName, clientId);
		sendIrcMessage(":" + _hostname + " 332 " + client.getNickname() + " " + channelName + " :No topic is set", clientId);
		sendIrcMessage(":" + _hostname + " 353 " + client.getNickname() + " = " + channelName + " :" + getUsersList(getChannelByName(channelName)), clientId);
		sendIrcMessage(":" + _hostname + " 366 " + client.getNickname() + " " + channelName + " :End of NAMES list", clientId);
	}
	return 1;
}

int	Manager::kickAction( void )
{
	std::cout << "Tas todo ze queres kickar quem crl" << std::endl;
	//	KICK <channel> <user> :<reason>
	return(1);
}

int	Manager::modeAction( void )
{
	std::cout << "modos obscuros de fazer cenas" << std::endl;
	return(1);
}

int	Manager::topicAction( void )
{
	std::cout << "A Gabi nao pode ouvir este topico" << std::endl;
	return(1);
}

int	Manager::inviteAction( void )
{
	std::cout << "tao, pega umas cervejinhas e buga" << std::endl;
	return(1);
}

int	Manager::privAction( void )
{
	std::cout << "Acho que o Mario esta com ciumes Gabi" << std::endl;
	return (1);
}
