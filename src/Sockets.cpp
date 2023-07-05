#include <Sockets.hpp>

Sockets::Sockets( std::string port, std::string password)
{
	std::cout << "Constructer Called" << std::endl;
	std::cout << "port: " << port << std::endl;
	std::cout << "Password: " << password << std::endl;
}

Sockets::~Sockets()
{
	std::cout << "Destructer Called" << std::endl;
}

// Take in account that the struct addrinfo defined in the <netdb.h> which means the members of the struct already
// exist. Thus we do not need to declare them prior.
addrinfo	*Sockets::getServerInfo()
{
	int	status = 0;
	struct	addrinfo *res, hints;

	hints.ai_flags = AI_PASSIVE; //Automatically fills in my ip for me
	hints.ai_family = AF_UNSPEC; //Means we don't care if we use IP4 or IP6 is compatible with both like this
	hints.ai_socktype = SOCK_STREAM; // Type of connection, receive and send 
	if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &res)))
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	return (res);
}