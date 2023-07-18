#include <Sockets.hpp>
#include <Clients.hpp>
#include <Manager.hpp>

void	Sockets::passwordCHeck(int _id)
{
	std::vector<Clients>::iterator iter = Manager::getClientById(_id);
	Clients& foundCLient = *iter;
	foundCLient.setPassword("password");
	if (foundCLient.getPassword() != this->_password)
	{
		close(_id);
		FD_CLR(_id, &_fdMaster);
		Manager::removeClient(_id);
	}
}

void	Sockets::handleMessage(int i, int read, char *buffer)
{
	buffer[read] = 0;
	std::cout << buffer << std::endl;
	for(int j = 0; j <= _fdMax; j++)
	{
		if (FD_ISSET(j, &_fdMaster))
		{
			// Lets get the client object from the vector
			std::vector<Clients>::iterator iter = Manager::getClientById(j);
			//Check if the client was added to the vector, thats the first if
			//Second If is to check if the client is already in the channel, if not we add it in the function
			// However the parser needs to run first so that we have all that information to add to the client
			Manager::parseCommands(iter, buffer, read); // Its empty for now, just layout func
			if (iter != Manager::getClients().end())
			{
				passwordCHeck(i);	
				Manager::firstTimeClient(iter);
			}
			// This for now is to send the messages without any kind of validation
			if (j != _fdSocket && j != i)
				if (send(j, buffer, read, 0) == -1)
					exit(Error::message("Error sending message"));
			// Its merely for testing purpuses. Use this as a base to send messages
		}
	}
}


void	Sockets::acceptConnection( void )
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
		if (newSocket > _fdMax) // Keep track of the max socket number
			_fdMax = newSocket;
		Manager::addClient(newSocket);
		std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " on socket " << newSocket << std::endl;
	}
}

void	Sockets::socketActivity(fd_set readFd)
{
	int		readbytes;
	char	buffer[MAX_READ + 1];

	for (int i = 0; i <= _fdMax; i++)
	{
		bzero(buffer, MAX_READ + 1);
		if (FD_ISSET(i, &readFd))
		{
			if (i == _fdSocket) // If its the master socket we got a new connection
				acceptConnection();
			else
				{
					if ((readbytes = recv(i, buffer, sizeof buffer, 0)) <= 0)
					{
						if (readbytes == 0)
							Manager::removeClient(i);
						else
							exit(Error::message("Receive Message failed"));
						close(i);
						FD_CLR(i, &_fdMaster); // Remove fd from master 
					}
					else
						handleMessage(i, readbytes, buffer);
				}
		}
	}
}

void	Sockets::HandleConnection( void )
{
	// We set the max fd in order to keep track of all the Highest file descriptor number, need for the 
	// select function loop. When we loop trought the fd set we will only loop trought the sockets that are active
	// and not all the sockets in the set
	_fdMax = _fdSocket; 
	fd_set	readFd; // This is the temporary set of sockets we will use to check for activity

	FD_ZERO(&_fdMaster); // Clear the socket set
	FD_SET(_fdSocket, &_fdMaster);	// Add master socket to set of sockets
	while (1)
	{
		readFd = _fdMaster; // Copy the master socket to the temporary set thats going to read the activity
		// wait until either socket has data ready to be recv() 
		if (select(_fdMax + 1, &readFd, NULL, NULL, NULL) == -1) // This is a blocking call, sellect will wait until there is activity on the socket
			exit(Error::message("Select failed"));
		// Check all the sockets for activity
		socketActivity(readFd);
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
