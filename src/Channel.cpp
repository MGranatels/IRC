#include <Channel.hpp>

Channel::Channel(std::string name)
{
	_name = name;
}

Channel::~Channel( void )
{
	std::cout << "Channel " << _name << " deleted" << std::endl;
}