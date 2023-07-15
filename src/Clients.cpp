#include <Clients.hpp>

Clients::Clients( int id ) : _id(id) {
}

Clients::~Clients( void ) {
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

void	Clients::initClient(std::string username, std::string nickname)
{
	this->_username = username;
	this->_nickcname = nickname;
}
