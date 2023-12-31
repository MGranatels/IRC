#include <Sockets.hpp>
#include <Channel.hpp>

int	main(int ac, char **av)
{
	if (ac != 3)
		return(Error::message("Incorrect number of arguments"));
	signal(SIGINT, handlesginal);
	signal(SIGQUIT, handlesginal);
	signal(SIGTERM, handlesginal);
	Sockets	server(av[1], av[2]);
	server.startConnection();
}

// 1. Handle the server response to send message to all clients via open terminal
