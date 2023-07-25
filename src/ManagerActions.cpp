#include <Manager.hpp>
#include <Channel.hpp>

std::vector<Clients> Manager::_clients;
std::vector<Channel> Manager::_channels;
std::string Manager::_hostname = ":localhost ";

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

// aqui podes passar mais parametros

int	Manager::joinAction( std::string channelName, int clientId )
{
	// First, check if the channel exists
	std::vector<Clients>::iterator iter = Manager::getClientById(clientId);
	Clients& client = *iter;

	if (!isValidChannel(channelName))
	{
		// Channel doesn't exist, so create it
		_channels.push_back(Channel(channelName));

		// Send the JOIN message to the client
		sendIrcMessage(":" + _hostname + " JOIN " + channelName, clientId);

		// Send the RPL_NOTOPIC (331) message to the client
		sendIrcMessage(":" + _hostname + " 331 " + client.getNickname() + " " + channelName + " :No topic is set", clientId);

		// Send the RPL_TOPIC (332) message to the client (replace "Channel topic here" with the actual topic)
		sendIrcMessage(":" + _hostname + " 332 " + client.getNickname() + " " + channelName + " :Channel topic here", clientId);
	}
	else
	{
		// The channel already exists, so send a message to notify the client about the join
		sendIrcMessage(":" + _hostname + " JOIN " + channelName, clientId);
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
