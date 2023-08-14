#include <Channel.hpp>

Channel::Channel(std::string name):
	_name(name) {
	this->_modes["i"] = MODE_NOT_SET;
	this->_modes["t"] = MODE_SET;
	this->_modes["k"] = MODE_NOT_SET;
	this->_modes["o"] = MODE_SET;
	this->_modes["l"] = MODE_NOT_SET;
	this->_modes["m"] = MODE_NOT_SET;
	this->_modes["b"] = MODE_NOT_SET;
	this->_modes["s"] = MODE_NOT_SET;
	std::cout << "Channel " << _name << " created" << std::endl;
}

Channel::Channel(std::string name, std::string topic):
	_name(name), _topic(topic) {
	this->_modes["i"] = MODE_NOT_SET;
	this->_modes["t"] = MODE_SET;
	this->_modes["k"] = MODE_NOT_SET;
	this->_modes["o"] = MODE_SET;
	this->_modes["l"] = MODE_NOT_SET;
	this->_modes["m"] = MODE_NOT_SET;
	this->_modes["b"] = MODE_NOT_SET;
	this->_modes["s"] = MODE_NOT_SET;
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

void Channel::removeInvitee(int clientId)
{
    removeElementFromVector(_inviteesIds, clientId);
}

void Channel::removeMuted(int clientId)
{
	removeElementFromVector(_mutedIds, clientId);
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

void Channel::addInvitee(int newClientId)
{
	_inviteesIds.push_back(newClientId);
}

void Channel::addMuted(int newClientId)
{
	_mutedIds.push_back(newClientId);
}

void	Channel::addSuperUser( std::string user ) {
	this->_superUser = user;
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

int	Channel::getClientsCount(void) const {
	return (this->_clientsIds.size());
}

std::string	Channel::getClientsCountStr(void) const {
	std::stringstream ss;

	ss << this->getClients().size();
	std::string numberClients = ss.str();
	return (numberClients);
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

bool Channel::isClientInvited( int clientId)
{
	std::vector<int>::iterator it = _inviteesIds.begin();
	for ( ; it != _inviteesIds.end(); it ++)
	{
		if (*it == clientId)
			return (true);
	}
	return (false);
}

bool Channel::isClientMuted( int clientId)
{
	std::vector<int>::iterator it = _mutedIds.begin();
	for ( ; it != _mutedIds.end(); it ++)
	{
		if (*it == clientId)
			return (true);
	}
	return (false);
}

bool Channel::isClientBanned(int clientId) {
	if (_bannedIds.find(clientId) != _bannedIds.end())
		return true;
	return false;
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

void	Channel::setPassword(std::string password) {
	_password = password;
}

std::string Channel::getPassword(void) const {
	return _password;
}

std::string Channel::getSuperUser(void) const {
	return this->_superUser;
}

std::string Channel::getChannelModes() {
    const char modeFlags[] = "tkolmbs";
    std::string modes;

    for (const char* flag = modeFlags; *flag; ++flag) {
        modes += (this->isModeSet(std::string(1, *flag))) ? ("+" + std::string(1, *flag) + " ") : ("-" + std::string(1, *flag) + " ");
    }
    return modes;
}

void	Channel::setLimit(int limit) {
	_limit = limit;
}

unsigned int Channel::getLimit(void) const {
	return _limit;
}

void Channel::addBanned(Clients &newClientId, Clients &BanSetterId) {

	std::time_t currentTime = std::time(NULL);
	std::string timestamp = std::ctime(&currentTime);
	size_t newlinePos = timestamp.find('\n');

	if (newlinePos != std::string::npos)
		timestamp.erase(newlinePos, 1);
	_bannedIds[newClientId.getId()].timestamp = timestamp;
	_bannedIds[newClientId.getId()].banMask = ":" + newClientId.getNickname() + "!" + BanSetterId.getUsername();
	_bannedIds[newClientId.getId()].banSetter = BanSetterId.getNickname();
}

void Channel::removeBanned(int clientId) {
	_bannedIds.erase(clientId);
}

void	Channel::removeSuperUser( void ) {
	this->_superUser = "";
}

void	Channel::removeAllOperators( void ) {
	this->_operatorsIds.clear();
}
