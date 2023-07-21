#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# define BACKLOG 10 // how many pending connections queue will hold
# define FD_MAX 10
# define MAX_READ	3500

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <iostream>
# include <cstdlib>
# include <arpa/inet.h>
# include <vector>
# include <iomanip>
# include <poll.h>
# include <cstring>
# include <fcntl.h>
# include <map>
# include <netinet/in.h>
# include <csignal>
# include <Error.hpp>
# include <utils.hpp>
# include <bits/stdc++.h>
# include <colors.h>

//Static backend handler
class Sockets
{
	private:
		int					_fdMax;
		int					_fdSocket;
		int					_ignoreSockets[FD_MAX];
		fd_set				_fdMaster; //  variable is used to store the set of socket file descriptors for which you want to check readability.
		const std::string	_port;
		const std::string	_password;

		int			bindSocketFD(struct addrinfo *serv);
		addrinfo	*setServerInfo();
		void		HandleConnection( void );
		void		handleMessage(int i, int read, char *buffer);
		void		acceptConnection( void );
		void		socketActivity(fd_set readFd);
		void		_select( void );
		bool		passwordCheck( int _id );

	public:
		Sockets( std::string port, std::string password);
		~Sockets();
		void	_init( void );
		void	startConnection( void );
		void	setIgnoredSockets(int fd);
		void	cleanSocket(int i);
		int		ignoreSocket(int i) const;
};


#endif