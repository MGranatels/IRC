#include <Channel.hpp>

Channel::Channel(std::string name):
	_name(name)
{
	std::cout << "Channel " << _name << " created" << std::endl;
}

Channel::Channel(std::string name, std::string topic):
	_name(name), _topic(topic)
{
	std::cout << "Channel " << _name << " created" << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel " << _name << " deleted" << std::endl;
}

//function to return users in channel minus the sender
std::vector<int> Channel::getUsersNoSender(int senderId) const
{
	std::vector<int> noSender(this->_usersIds);

	std::vector<int>::iterator it = std::remove(noSender.begin(), noSender.end(), senderId);
    noSender.erase(it, noSender.end());
	return noSender;
}

//function to remove users from channel
void Channel::removeUser(int UserId)
{
    std::vector<int>::iterator it = std::remove(_usersIds.begin(), _usersIds.end(), UserId);
    _usersIds.erase(it, _usersIds.end());
}

//returns the users in a channel but just a const reference, i.e we can only edit this inside the class
const std::vector<int>& Channel::getUsers(void) const
{
	return (this->_usersIds);
}

//function to add users to channel
void Channel::addUser(int newUserId)
{
	_usersIds.push_back(newUserId);
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