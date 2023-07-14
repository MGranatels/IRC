#ifndef	MANAGER_HPP
# define MANAGER_HPP

# include <Sockets.hpp>
# include <Clients.hpp>
# include <Channel.hpp>

class Manager
{
	private:
		std::vector <Channel> _Channels;
        std::vector <Clients> _Clients;

    public:
    
};

#endif