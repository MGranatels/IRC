#include <Manager.hpp>

int							Manager::_awake = 0;
std::vector<Clients> 		Manager::_clients;
std::vector<Channel> 		Manager::_channels;
std::string 				Manager::hostname = "localhost";
std::string 				Manager::_serverName = "irc";
std::map<std::string, ActionFunction> Manager::actionMap;
std::map<std::string, ModeFunction> Manager::modeMap;


int	Manager::addClient(int id)
{
	if (Manager::getClientById(id) != _clients.end())
		return 1;
	_clients.push_back(Clients(id));
	std::cout << LightGreen << "Client Succesfully Added" << NC << std::endl;
	return (0);
}

void	Manager::removeClient(int id)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getId() == id)
		{
			removeClientFromAllChannels(id);
			_clients.erase(_clients.begin() + i);
			std::cout << LightRed << "Client Succesfully Removed" << NC << std::endl;
			close(id);
			return ;
		}
	}
	// std::cout << "Client Not Found" << std::endl;
}

std::vector<Clients>::iterator Manager::getClientById(int id)
{
	std::vector<Clients>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
		if (it->getId() == id)
			break;
	return (it);
}

const std::vector<Clients>& Manager::getClients() {
	return _clients;
}

//Channel names are case insensitive
Channel&	Manager::getChannelByName(std::string name)
{
	long unsigned int  i;
	for (i = 0; i < _channels.size(); i++)
	{
		if (toUpperCase(_channels[i].getName()) == toUpperCase(name))
			break ;
	}
	return (_channels[i]);
}

// The isValidChannel function should be implemented to correctly validate whether the provided channel
// name is valid or not. The function should check if the channel name conforms to the rules of IRC
// channel naming, such as starting with the appropriate character specified in CHANTYPES and not
// containing any spaces or special characters.
int	Manager::isValidChannel(std::string name)
{
	long unsigned int  i;
	if (name[0] != '#')
		return NOT_VALID;
	for (i = 0; i < _channels.size(); i++)
	{
		if (toUpperCase(_channels[i].getName()) == toUpperCase(name))
			return (CREATED) ;
	}
	return (VALID_NAME);
}

std::string	Manager::getUsersList(Channel &Channel)
{
	std::string usersList;

	for (std::vector<int>::size_type i = 0; i < Channel.getClients().size(); i++)
	{
		std::vector<Clients>::iterator iter = Manager::getClientById(Channel.getClients()[i]);
		Clients& client = *iter;
		//Lets get the user operator and add a @
		if (checkChannelOp(Channel, client.getId()))
			usersList += "@" + client.getNickname() + " ";
		else
			usersList += "%" + client.getNickname() + " ";
	}
	return usersList;
}

int	Manager::firstTimeClient(std::vector<Clients>::iterator it)
{
	Clients& client = *it;
	// After the parse does its work we assign the username and nickname
	// They should be in a list of commands, like a vector of strings
	// We also need to check if the password is correct
	// We should also add
	std::cout << client.getId() << std::endl;
	if (!client.getUsername().empty())
		return 0;
	return (1);
}

//id: the user receiver
int	Manager::sendIrcMessage(std::string message, int clientId)
{
	message = message + "\r\n";
	std::cout << "Sending message: " << message << std::endl;
	if (send(clientId, message.c_str(), message.length(), 0) == -1)
		exit(Error::message("Error sending message"));
	return 0;
}

bool	Manager::checkNickName(Clients client) {
	printMessage("Checking Nickname...", LightRed);
	if (!isNickValid(client.getNickname()))	{
			std::cout << "Erroneus Nickname" << std::endl;
			sendIrcMessage(formatMessage(client, ERRONEUSNICKNAME) + " :Erroneus nickname", client.getId());
			return false;
	}
	for (std::vector<Clients>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getId() == client.getId())
			continue ;
		if (it->getNickname() == client.getNickname()) {
			std::cout << "Nickname already in use" << std::endl;
			sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " :Nickname is already in use", client.getId());
			return false;
		}
	}
	printMessage("NickName Validated", LightGreen);
	return true;
}

bool	Manager::checkPassword(Clients client, std::string password) {
	if (client.getPassword() != password || client.getPassword().empty())
	{
		printMessage("Password Incorrect or empty, Retry Password", Red);
		sendIrcMessage(formatMessage(client, PASSWDMISMATCH) + ":Password required", client.getId());
		return false;
	}
	return true;
}

void	Manager::setChannOpps(Clients& client)
{
	sendIrcMessage(formatMessage(client, WELCOME_MESSAGE) + " :Welcome to the Darkest Region of the Internet", client.getId());
	sendIrcMessage(formatMessage(client, CHANNEL_OPPS) + " :CHANTYPES=#", client.getId());
	sendIrcMessage(formatMessage(client, CHANNEL_OPPS) + " :CHANMODES=i,t,k,o,l", client.getId());
	Bot myBot(client);
	client.setOppChannel(true);
}

Clients&	Manager::getClientByNick(std::string nickname)
{
	std::vector<Clients>::iterator it = _clients.begin();

	for (; it < _clients.end(); it++)
	{
		if (it->getNickname() == nickname)
			return (*it);
	}
	return (*it);
}

bool		Manager::isValidClient(std::string nickname)
{
	std::vector<Clients>::iterator it = _clients.begin();

	for (; it < _clients.end(); it++)
	{
		if (it->getNickname() == nickname)
			return (true);
	}
	return (false);
}

bool		Manager::isValidClient(int	id)
{
	std::vector<Clients>::iterator it = _clients.begin();

	for (; it < _clients.end(); it++)
	{
		if (it->getId() == id)
			return (true);
	}
	return (false);
}

void	Manager::removeClientFromAllChannels(int clientId)
{
	std::vector<Channel>::iterator it = _channels.begin();
	Clients &client = *getClientById(clientId);
	for ( ; it != _channels.end(); it++)
	{
		if (it->isClientInChannel(clientId))
		{
			BroadcastMessageChan(*it, formatMessage(client) + " QUIT :has quit");
			it->removeClient(clientId);
			if (it->isClientOperator(clientId))
			{
				it->removeOperator(clientId);
				if (it->getOperators().empty())
					it->addOperator(*(it->getClients().begin()));
			}
			messageUpdateUserList(*it, client);
		}
	}
}

std::vector<int> Manager::getAllClientsIds( void )
{
	std::vector<int> clients;
	std::vector<Clients>::iterator it = _clients.begin();

	for ( ; it != _clients.end(); it++)
		clients.push_back(it->getId());
	return (clients);
}
// Lets make a function that checks all channels for the channel operators and returns the total of chanops
std::string	Manager::getAllChanOps( void )
{
	std::stringstream ss;
	int total = 0;
	std::vector<Channel>::iterator it = _channels.begin();

	for ( ; it != _channels.end(); it++)
		total += it->getOperators().size();
	ss << total;
	return (ss.str());
}

std::string	Manager::getUnkownClients( void )
{
	std::stringstream ss;
	int total = 0;
	std::vector<Clients>::iterator it = _clients.begin();

	for ( ; it != _clients.end(); it++)
		if (it->getClientSettings() == false)
			total++;
	ss << total;
	return (ss.str());
}

std::map<std::string, std::string> Manager::getChannelNameAndKey(std::vector<std::string>& cmd)
{
    std::map<std::string, std::string> result;
    std::string channel, key;
    std::istringstream channelStream(cmd[1]);
    if (cmd.size() < 3)
    {
		 while (std::getline(channelStream, channel, ','))
		 	result[channel] = "";
        return result;
    }
    std::istringstream keyStream(cmd[2]);
    while (std::getline(channelStream, channel, ','))
    {
        if (std::getline(keyStream, key, ','))
            result[channel] = key;
        else
            result[channel] = ""; // or you can omit this line if you prefer
    }

    return result;
}


void Manager::leaveAllChannels(Clients& client) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if ((it)->isClientInChannel(client.getId())) {
            BroadcastMessageChan(*it, formatMessage(client, "PART") + " " + (it)->getName());
            // Send a PART message to the client to indicate they left the channel.
            sendIrcMessage(formatMessage(client) + " PART " + (it)->getName(), client.getId());
            BroadcastMessageChan(*it, formatMessage(client, "QUIT") + " :has quit the channel");
            // Remove the user from the channel
            (it)->removeClient(client.getId());
            messageUpdateUserList(*it, client);
        }
    }
}
