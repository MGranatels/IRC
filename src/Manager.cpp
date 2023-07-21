#include <Manager.hpp>

std::vector<Clients> Manager::_clients;
std::string Manager::_hostname = ":localhost ";
std::map<std::string, Manager::MemberFunctionPointer> Manager::_chanActions;

void	Manager::setChanActions( void )
{
	// Channel Operators
	//	KICK    - Eject a client from the channel
	//	MODE    - Change the channel's mode
	//	INVITE  - Invite a client to an invite-only channel (mode +i)
	//	TOPIC   - Change the channel topic in a mode +t channel

	// CALADO:

	_chanActions["KICK"] = &Manager::kickAction;
    _chanActions["MODE"] = &Manager::modeAction;
    _chanActions["TOPIC"] = &Manager::topicAction;
    _chanActions["INVITE"] = &Manager::inviteAction;
}

int	Manager::kickAction( void )
{
    std::cout << "Tas todo ze queres kickar quem crl" << std::endl;
    return(1);
}

int	Manager::modeAction( void )
{
    std::cout << "modos obscuros de fazer cenas" << std::endl;
    return(1);
}

int	Manager::topicAction( void )
{
    std::cout << "A Gabi nao pode ouvir este topico" << std::endl;
    return(1);
}

int	Manager::inviteAction( void )
{
    std::cout << "tao, pega umas cervejinhas e buga" << std::endl;
    return(1);
}

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


void	Manager::sendIrcMessage(std::string message, int id)
{
	std::string msg = _hostname + message + "\r\n";
	std::cout << "Sending message: " << msg << std::endl;
	if (send(id, msg.c_str(), msg.length(), 0) == -1)
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