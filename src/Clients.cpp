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

//test

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

void	Clients::setCmd( std::vector<std::string> cmd ) {
	this->_cmd.clear();
	this->_cmd = cmd;
}

std::vector<std::string>	Clients::getCmd( void ) const {
	return (this->_cmd);
}

// Remove argument int cmd from vector
void	Clients::removeCmd( void ) {
	if (_cmd.size() > 1 && containsSubstring(_cmd[1], "irc"))
		this->_cmd.erase(this->_cmd.begin() + 1);
	for (unsigned int i = 0; i < _cmd.size(); i++)
		this->_cmd[i] = removeCharacter(this->_cmd[i], ':');
}