#include <Manager.hpp>

std::vector<Clients> Manager::_clients;
std::string Manager::_hostname = ":localhost ";

void	Manager::addClient(int id)
{
	_clients.push_back(Clients(id));
	std::cout << "Client Succesfully Added" << std::endl;
}

void	Manager::removeClient(int id)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getId() == id)
		{
			_clients.erase(_clients.begin() + i);
			std::cout << "Client Succesfully Removed" << std::endl;
			return ;
		}
	}
	std::cout << "Client Not Found" << std::endl;
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

int	Manager::firstTimeClient(std::vector<Clients>::iterator it)
{
	Clients& client = *it;
	// After the parse does its work we assign the username and nickname
	// They should be in a list of commands, like a vector of strings
	// We also need to check if the password is correct
	// We should also add 
	if (!client.getUsername().empty())
		return 0;
	client.setUsername("teste");
	return (1);
}

void	Manager::setChanop( void )
{
	// Channel Operators
	//	KICK    - Eject a client from the channel
	//	MODE    - Change the channel's mode
	//	INVITE  - Invite a client to an invite-only channel (mode +i)
	//	TOPIC   - Change the channel topic in a mode +t channel

	this->_chanoper.push_back("/KICK");
	this->_chanoper.push_back("/MODE");
	this->_chanoper.push_back("/INVITE");
	this->_chanoper.push_back("/TOPIC");
}