#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# define BACKLOG 10 // how many pending connections queue will hold
# define MAX_CLIENTS 10
# define MAX_READ	3500

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
#include <netinet/in.h>

//Static backend handler
class Sockets 
{
	private:
		const std::string	_port;
		const std::string	_password;

	public:
		addrinfo			*setServerInfo();
		Sockets( std::string port, std::string password);
		~Sockets();
		void	_init( void );
};

void	ft_bzero(void *s, size_t n);

#endif