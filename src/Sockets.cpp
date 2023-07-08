#include <Sockets.hpp>

Sockets::Sockets( std::string port, std::string password): 
	_port(port), _password(password)
{
	std::cout << "Constructer Called" << std::endl;
	std::cout << "port: " << _port << std::endl;
	std::cout << "Password: " << _password << std::endl;
}

Sockets::~Sockets()
{
	std::cout << "Destructer Called" << std::endl;
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
	if ((status = getaddrinfo(NULL, "6667", &hints, &res)) != 0)
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	return (res);
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

	serv = setServerInfo();

	freeaddrinfo(serv);
}