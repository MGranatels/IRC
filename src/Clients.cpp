#include <Clients.hpp>

Clients::Clients( int id ) : _id(id) {
	this->_opChannel = false;
	this->_clientSettings = false;
}

Clients::~Clients( void ) {
}

//Setters
void		Clients::setId( int id ) {
	this->_id = id;
}

void	Clients::setNickname( std::string nickname ) {
	this->_nickcname = nickname;
	std::cout << Red << this->_nickcname << NC << std::endl;
}

void	Clients::setUsername( std::string username ) {

	this->_username = username;
}

void	Clients::setPassword( std::string password ) {
	this->_password = password;
}


void	Clients::setOppChannel( bool opChannel) {
	this->_opChannel = opChannel;
}

void	Clients::setClientSettings( bool clientSettings ) {
	this->_clientSettings = clientSettings;
}

//Getters

int			Clients::getId( void ) const {
	return (this->_id);
}


std::string	Clients::getNickname( void ) const {
	return (this->_nickcname);
}

std::string	Clients::getUsername( void ) const {
	return (this->_username);
}

std::string	Clients::getPassword( void ) const {
	return (this->_password);
}

void	Clients::initClient(std::string username, std::string nickname)
{
	this->_username = username;
	this->_nickcname = nickname;
}

bool	Clients::getClientSettings( void ) const {
	return (this->_clientSettings);
}

bool	Clients::getOppChannel( void ) const {
	return (this->_opChannel);
}