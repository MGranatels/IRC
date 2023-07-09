#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# define BACKLOG 10 // how many pending connections queue will hold
# define FD_MAX 10
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
#include <Error.hpp>
#include <utils.hpp>
#include <csignal>

//Static backend handler
class Sockets 
{
	private:
		fd_set				_fdMaster; //  variable is used to store the set of socket file descriptors for which you want to check readability.
		const std::string	_port;
		const std::string	_password;
		int					_fdSocket;
		int					bindSocketFD(struct addrinfo *serv);
		addrinfo			*setServerInfo();
		void	HandleConnection( void );
		void	sendMessage(int fd_max, int i, int read, char *buffer);
		void	acceptConnection(int &max_socket);
		void	socketActivity(fd_set readFd, int &fd_max);
		void	_select( void );

	public:
		Sockets( std::string port, std::string password);
		~Sockets();
		void	_init( void );
		void	startConnection( void );

};


#endif