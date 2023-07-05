#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# define BACKLOG 10 // how many pending connections queue will hold

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <vector>
#include <iomanip>
#include <poll.h>
#include <cstring>
#include <fcntl.h>
#include <map>

//Static backend handler
class Sockets 
{
	private:
		addrinfo			*getServerInfo();
		const std::string	_port;
		const std::string	_password;

	public:
		Sockets( std::string port, std::string password);
		~Sockets();
};

#endif