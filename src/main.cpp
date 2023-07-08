#include <Sockets.hpp>
#include <Error.hpp>

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	int		fd_socket;
	int		max_socket; // Needs an empty socket so we give it the max value
	fd_set	nb_accept;
	fd_set	nb_reject; // Necessary for select function (works like pool)
	struct	addrinfo	*res;
	// struct	sockaddr_in	test;

	// ft_bzero(&test, sizeof(test));

	if (ac != 3)
		return(Error::message("Incorrect number of arguments"));
	Sockets this_class(av[1], av[2]);
	res = this_class.setServerInfo();

	fd_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	// fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	max_socket = fd_socket;
	// Just for test porpuses we will use addrinfo
	// test.sin_family = AF_INET;
	// test.sin_port = htons(atoi(av[1]));
	// test.sin_addr.s_addr = htonl(INADDR_ANY);

	// if (bind(fd_socket, (const struct sockaddr *) &test, sizeof(test)) < 0)
	// 	return (2); //! MENSAGEM DE ERRO AQUI
	
	bind(fd_socket, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res); // free the linked list

	if (listen(fd_socket, MAX_CLIENTS) < 0)
		return (2); //! MENSAGEM DE ERRO AQUI

	// This is like bzero for this type of variable. REmoving garbage values
	FD_ZERO(&nb_accept);
	FD_SET(fd_socket, &nb_accept);
	while (1)
	{
		nb_reject = nb_accept;
		// wait until either socket has data ready to be recv()d (timeout 10.5 secs)
		select(max_socket + 1, &nb_reject, NULL, NULL, NULL);
		for (int client_socket = 0; client_socket < MAX_CLIENTS; client_socket++)
		{
			//Check 3 cases:
			// Check if there is any activity on the channel
			if (FD_ISSET(client_socket, &nb_reject))
			{
				if (client_socket == fd_socket)
				{
					int	Nclient = accept(fd_socket, NULL, NULL);
					if (( Nclient < 0))
						return(Error::message("Conection was rejected from server"));
					FD_SET(Nclient, &nb_accept);
					if (Nclient > max_socket)
						max_socket = Nclient;
				}
				else
				{
					char	buff[MAX_READ];
					int	read = recv(client_socket, buff, MAX_READ - 1, 0);
					if (read == -1)
						return(Error::message("Could not read message from client"));
					// This means we got a disconnection from server and were removing the client connection
					// and closing its open FD socket
					else if (read == 0)
					{
						FD_CLR(client_socket, &nb_accept);
						write(1, "DEBUG", 6);
					}
					else
					{
						write(1, "DEBUG2", 7);
						buff[read] = 0;
						send(client_socket, buff, read, 0);
						std::cout << buff << std::endl;
					}

				}
			}
			// Someone wants to send a message
			// Disconections: Someone disconects from server
		}
	}
	return 0;
}