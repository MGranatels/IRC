#include <Sockets.hpp>

Sockets::Sockets( std::string port, std::string password): _port(port), _password(password)
{
	_init();
}

Sockets::~Sockets()
{
	std::cout << "End of Server Connection" << std::endl;
}

// Take in account that the struct addrinfo defined in the <netdb.h> which means the members of the struct already
// exist. Thus we do not need to declare them prior.
// Here’s a sample call if you’re a server who wants to listen on your host’s IP address, port 3490. Note that
// this doesn’t actually do any listening or network setup; it merely sets up structures we’ll use later:
addrinfo	*Sockets::setServerInfo()
{
	int	status = 0;
	struct	addrinfo hints;
	struct	addrinfo *res;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; //Means we don't care if we use IP4 or IP6 is compatible with both like this
	hints.ai_socktype = SOCK_STREAM; // Type of connection, receive and send 
	hints.ai_flags = AI_PASSIVE; //Automatically fills in my ip for me
	if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &res)) != 0)
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	return (res);
}

int	Sockets::bindSocketFD(struct addrinfo *serv)
{
	int	yes = 1;

	// Create socket and Bind it. We use a loop to check if we can bind to the socket continusly until we succeed
	for (struct addrinfo *tmp = serv; tmp; tmp = tmp->ai_next)
	{
		int	fd_socket = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (fd_socket == -1 || tmp->ai_family != AF_INET6)
			continue ;
		if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			close(fd_socket);
			freeaddrinfo(serv);
			exit(Error::message("Set Sock Option: Failed to free up port"));
		}
		if (bind(fd_socket, tmp->ai_addr, tmp->ai_addrlen) == -1)
		{
			close(fd_socket);
			freeaddrinfo(serv);
			exit(Error::message("bind: Failed to bind socket"));
		}
		else
		{
			std::cout << fd_socket << std::endl;
			return (fd_socket);
		}
	}
	return (-1);
}
// Get address info function:
// int getaddrinfo(const char *node, // e.g. "www.example.com" or IP
// const char *service, // e.g. "http" or port number
// const struct addrinfo *hints,
// struct addrinfo **res);
// You give this function three input parameters, and it gives you a pointer to a linked-li
// The node parameter is the host name to connect to, or an IP address.
// Next is the parameter service, which can be a port number, like “80”, or the name of a particular service
// (found in The IANA Port List1
// or the /etc/services file on your Unix machine) like “http” or “ftp” or
// “telnet” or “smtp” or whatever.
// Finally, the hints

void		Sockets::_init( void )
{
	struct addrinfo	*serv;

	if (!IsDigit(_port) || atoi(_port.c_str()) < 0 || atoi(_port.c_str()) > 65535)
		exit(Error::message("Port Number out of range or incorrecly formatted"));
	
	if(_password.empty())
		exit(Error::message("Password Cannot be Empty"));
	for (unsigned int i = 0; i < _password.length(); i++)
		if (isspace(_password[i]))
			exit(Error::message("Password must not contain Spaces"));
	serv = setServerInfo();
	_fdSocket = bindSocketFD(serv);
	freeaddrinfo(serv);
}
