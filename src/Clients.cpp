#include <Clients.hpp>

std::vector<Clients *> Clients::_clients;


Clients::Clients( int id ) : _id(id)
{
	std::cout << "Client " << id << " created" << std::endl;
	addClient(this);
}

Clients::~Clients( void )
{
	std::cout << "Client " << _id << " destroyed" << std::endl;
}

//Setters
void		Clients::setId( int id )
{
	this->_id = id;
}

void	Clients::setNickname( std::string nickname )
{
	this->_nickcname = nickname;
}

void	Clients::setUsername( std::string username )
{
	this->_username = username;
}

//Getters

int			Clients::getId( void ) const
{
	return (this->_id);
}

std::string	Clients::getNickname( void ) const
{
	return (this->_nickcname);
}

std::string	Clients::getUsername( void ) const
{
	return (this->_username);
}


// Set the getter to get the vector of clients, in order to manipulate, add, remove, etc.
Clients* Clients::getClientById(int id)
{
	for (unsigned int i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getId() == id)
			return (this->_clients[i]);
	}
	std::cout << "Client not found" << std::endl;
	return (NULL);
}

void	Clients::addClient(Clients *client)
{
	this->_clients.push_back(client);
	std::cout << "Client Succesfully Added" << std::endl;
}

void	Clients::removeClient(int id)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getId() == id)
		{
			delete(_clients[i]);
			_clients.erase(_clients.begin() + i);
		}
	}
	std::cout << "Client Succesfully Removed" << std::endl;
}