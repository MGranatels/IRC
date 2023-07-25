#include <Manager.hpp>

std::vector<Clients> Manager::_clients;
std::string Manager::_hostname = ":localhost ";

// aqui podes passar mais parametros

int	Manager::runChanActions( std::vector<std::string> splits )
{
	if (splits[0].compare("JOIN") == 0)
		return( Manager::joinAction() );
	else if (splits[0].compare("KICK") == 0)
		return( Manager::kickAction() );
	else if (splits[0].compare("MODE") == 0)
		return( Manager::modeAction() );
	else if (splits[0].compare("TOPIC") == 0)
		return( Manager::topicAction() );
	else if (splits[0].compare("INVITE") == 0)
		return( Manager::inviteAction() );
	else if (splits[0].compare("PRIVMSG") == 0)
		return( Manager::privAction() );
	else
		std::cout << "Action not found" << std::endl;
	return (-1);
}

// aqui podes passar mais parametros

int	Manager::joinAction( void )
{
	std::cout << "Acho que o Mario esta com ciumes Gabi" << std::endl;
	return (1);
}

int	Manager::kickAction( void )
{
	std::cout << "Tas todo ze queres kickar quem crl" << std::endl;
	//	KICK <channel> <user> :<reason>

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

int	Manager::privAction( void )
{
	std::cout << "Acho que o Mario esta com ciumes Gabi" << std::endl;
	return (1);
}
