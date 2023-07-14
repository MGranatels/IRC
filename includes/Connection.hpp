#ifndef	CONNECTION_HPP
# define CONNECTION_HPP

# include <Sockets.hpp>

class	Connection
{
	private:
		fd_set	_fd_read; //  variable is used to store the set of socket file descriptors for which you want to check readability.
	public:
		Connection( void );
		~Connection( void );

}