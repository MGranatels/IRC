#include <Sockets.hpp>
#include <Clients.hpp>
#include <Manager.hpp>

int	Sockets::passwordCheck(int _id)
{
	std::vector<Clients>::iterator iter = Manager::getClientById(_id);
	Clients& foundClient = *iter;

	if (foundClient.getClientSettings() == true)
		return 1;
	printMessage("Waiting for password Verification... Please Hold...", Cyan);
	if (foundClient.getPassword().empty() == true) {
		printMessage("Password is empty, Trying to retrive password...", Red);
		return 0;
	}
	else if (foundClient.getPassword() != this->_password) {
		printMessage("Password Incorrect, disconnecting from server...", Red);
		//Manager::sendIrcMessage("PRIVMSG NickServ :IDENTIFY <1234>\r\n", _id);		//cleanSocket(_id);
		return 0;
	}
	foundClient.setClientSettings(true);
	if (!Manager::checkNickName(_id, foundClient.getNickname()))
		return 0;
	return (printMessage("Password Correct!!", Green));
}


void	Sockets::handleMessage(int i, int read, char *buffer)
{
	buffer[read] = 0;
	std::string str(buffer);
	std::vector<std::string> splits = split(str, "\r\n\t ");
	// for(int j = 0; j <= _fdMax; j++)
	// {
		if (FD_ISSET(i, &_fdMaster))
		{
			std::vector<Clients>::iterator iter = Manager::getClientById(i);
			if (iter != Manager::getClients().end())
				if (!Manager::checkClientData(splits, iter))
					passwordCheck(i);
			// This for now is to send the messages without any kind of validation
			std::cout << Manager::runChanActions(splits) << std::endl;
			if (i != _fdSocket)
				if (send(i, buffer, read, 0) == -1)
					exit(Error::message("Error sending message"));
			// Its merely for testing purpuses. Use this as a base to send messages
		}
	// }
}


void	Sockets::acceptConnection( void )
{
	int					newSocket;
	struct sockaddr_in	clientAddr;
	socklen_t			clientSize = sizeof(clientAddr);

	newSocket = accept(_fdSocket, (struct sockaddr *)&clientAddr, &clientSize);
	if (newSocket == -1)
		exit(Error::message("Could not accept Client connection"));
	FD_SET(newSocket, &_fdMaster); // Add new socket to set of sockets
	if (newSocket > _fdMax) // Keep track of the max socket number
		_fdMax = newSocket + 1;
	Manager::addClient(newSocket);
}

void	Sockets::socketActivity(fd_set readFd)
{
	int		readbytes;
	char	buffer[MAX_READ + 1];

	for (int i = 0; i <= _fdMax; i++)
	{
		bzero(buffer, MAX_READ + 1);
		if (ignoreSocket(i) == 1)
			continue ;
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

int	Sockets::ignoreSocket(int i) const {
	if (_ignoreSockets[i] == 1)
		return (1);
	return (0);
}

void	Sockets::cleanSocket(int i)
{
	close(i);
	FD_CLR(i, &_fdMaster); // Remove fd from master
	Manager::removeClient(i);
}