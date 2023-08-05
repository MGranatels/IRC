#include <Channel.hpp>

Channel::Channel(std::string name):
	_name(name) {
	this->_modes["i"] = MODE_NOT_SET;
	this->_modes["t"] = MODE_SET;
	this->_modes["k"] = MODE_NOT_SET;
	this->_modes["o"] = MODE_SET;
	this->_modes["l"] = MODE_NOT_SET;
	std::cout << "Channel " << _name << " created" << std::endl;
}

Channel::Channel(std::string name, std::string topic):
	_name(name), _topic(topic) {
	this->_modes["i"] = MODE_NOT_SET;
	this->_modes["t"] = MODE_SET;
	this->_modes["k"] = MODE_NOT_SET;
	this->_modes["o"] = MODE_SET;
	this->_modes["l"] = MODE_NOT_SET;
	std::cout << "Channel " << _name << " created" << std::endl;
}

Channel::~Channel( void ) {
	std::cout << "Channel " << _name << " deleted" << std::endl;
}

//function to return users in channel minus the sender
std::vector<int> Channel::getClientsNoSender(int senderId) const
{
	std::vector<int> noSender(this->_clientsIds);

	removeElementFromVector(noSender, senderId);
	return noSender;
}

//function to remove users from channel
void Channel::removeClient(int clientId)
{
    removeElementFromVector(_clientsIds, clientId);
}

//returns the users in a channel but just a const reference, i.e we can only edit this inside the class
const std::vector<int>& Channel::getClients(void) const
{
	return (this->_clientsIds);
}

//function to add users to channel
void Channel::addClient(int newClientId)
{
	_clientsIds.push_back(newClientId);
}

void Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void Channel::setName(std::string name)
{
	_name = name;
}

std::string Channel::getTopic(void) const
{
	return (_topic);
}

std::string Channel::getName(void) const
{
	return (_name);
}

const std::vector<int>& Channel::getOperators(void) const
{
	return (this->_operatorsIds);
}

void Channel::addOperator(int operatorId)
{
	this->_operatorsIds.push_back(operatorId);
}

void Channel::removeOperator(int operatorId)
{
	removeElementFromVector(_operatorsIds, operatorId);
}

bool Channel::isClientInChannel(int clientId)
{
	std::vector<int>::iterator it = _clientsIds.begin();
	for ( ; it != _clientsIds.end(); it ++)
	{
		if (*it == clientId)
			return (true);
	}
	return (false);
}

bool Channel::isClientOperator( int clientId)
{
	std::vector<int>::iterator it = _operatorsIds.begin();
	for ( ; it != _operatorsIds.end(); it ++)
	{
		if (*it == clientId)
			return (true);
	}
	return (false);
}

void	Channel::setMode(const std::string& mode) {
	_modes[mode] = MODE_SET;
}

// Function to unset a channel mode
void	Channel::unsetMode(const std::string& mode) {
	_modes[mode] = MODE_NOT_SET;
}

// Function to check if a channel mode is set
bool Channel::isModeSet(const std::string& mode) {
	return _modes[mode] == MODE_SET;
}

std::map<std::string, ChannelModeStatus> Channel::getModes() const {
    return _modes;
}

void Channel::setPassword(std::string password) {
	_password = password;
}

std::string Channel::getPassword(void) const {
	return _password;
}

void Channel::setLimit(int limit) {
	_limit = limit;
}

unsigned int Channel::getLimit(void) const {
	return _limit;
}

