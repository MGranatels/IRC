#include <Sockets.hpp>


void	Sockets::sendMessage(int fd_max, int i, int read, char *buffer)
{
	std::cout << "Message received from socket " << i << ": " << buffer;
	buffer[read] = 0;
	for(int j = 0; j <= fd_max; j++)
	{
		if (FD_ISSET(j, &_fdMaster))
		{
			// except the listener and ourselves
			if (j != _fdSocket && j != i)
			{
				if (send(j, buffer, read, 0) == -1)
					exit(Error::message("Error sending message"));
			}
		}
	}
}

void	Sockets::acceptConnection(int &max_socket)
{
	int					newSocket;
	struct sockaddr_in	clientAddr;
	socklen_t			clientSize = sizeof(clientAddr);

	newSocket = accept(_fdSocket, (struct sockaddr *)&clientAddr, &clientSize);
	if (newSocket == -1)
		exit(Error::message("Could not accept Client connection"));
	else
	{
		FD_SET(newSocket, &_fdMaster); // Add new socket to set of sockets
		if (newSocket > max_socket) // Keep track of the max socket number
			max_socket = newSocket;
		std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " on socket " << newSocket << std::endl;
	}
}

void	Sockets::socketActivity(fd_set readFd, int &fd_max)
{
	int		readbytes;
	char	buffer[MAX_READ + 1];

	for (int i = 0; i <= fd_max; i++)
	{
		bzero(buffer, MAX_READ + 1);
		if (FD_ISSET(i, &readFd))
		{
			if (i == _fdSocket) // If its the master socket we got a new connection
				acceptConnection(fd_max);
			else
				{
					if ((readbytes = recv(i, buffer, sizeof buffer, 0)) <= 0)
					{
						if (readbytes == 0)
							std::cout << "Connection on Socket " << i << " Disconnected from Server" << std::endl;
						else
							exit(Error::message("Receive Message failed"));
						close(i);
						FD_CLR(i, &_fdMaster); // Remove fd from master 
					}
					else
						sendMessage(fd_max, i, readbytes, buffer);
				}
		}
	}
}

void	Sockets::HandleConnection( void )
{
	int		fd_max = _fdSocket; // So far its the one
	fd_set	readFd; // This is the temporary set of sockets we will use to check for activity

	FD_ZERO(&_fdMaster); // Clear the socket set
	FD_SET(_fdSocket, &_fdMaster);	// Add master socket to set of sockets
	while (1)
	{
		readFd = _fdMaster; // Copy the master socket to the temporary set thats going to read the activity
		// wait until either socket has data ready to be recv() 
		if (select(fd_max + 1, &readFd, NULL, NULL, NULL) == -1) // This is a blocking call, sellect will wait until there is activity on the socket
			exit(Error::message("Select failed"));
		// Check all the sockets for activity
		socketActivity(readFd, fd_max);
	}
}

void	Sockets::startConnection( void )
{
	// This will do the pool handling meeaning it will check for any activity on the socket
	if (_fdSocket == -1)
		exit(Error::message("Socket was not initialized"));
	if (listen(_fdSocket, BACKLOG) == -1)
		exit(Error::message("Listen failed"));
	std::cout << "Server is listening on port " << _port << std::endl;
	HandleConnection();
}