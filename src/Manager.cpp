#include <Manager.hpp>

int	Manager::addClient(int id)
{
	if (Manager::getClientById(id) != _clients.end())
		return 1;
	_clients.push_back(Clients(id));
	_clients.back().setClientSettings(false);
	std::cout << LightGreen << "Client Succesfully Added" << NC << std::endl;
	return (0);
}

void	Manager::removeClient(int id)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getId() == id)
		{
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

	for (std::vector<int>::size_type i = 0; i < Channel.getUsers().size(); i++)
	{
		std::vector<Clients>::iterator iter = Manager::getClientById(Channel.getUsers()[i]);
		Clients& client = *iter;
		usersList += client.getNickname() + " ";
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
void	Manager::sendIrcMessage(std::string message, int clientId)
{
	message = message + "\r\n";
	std::cout << "Sending message: " << message << std::endl;
	if (send(clientId, message.c_str(), message.length(), 0) == -1)
		exit(Error::message("Error sending message"));
}

bool	Manager::checkNickName(int id, std::string nickName) {
	for (std::vector<Clients>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getId() == id)
			continue ;
		if (it->getNickname() == nickName) {
			std::cout << "Nickname already in use" << std::endl;
			return false;
		}
	}
	return true;
}
