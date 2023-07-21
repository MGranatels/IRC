#ifndef  MANAGER_HPP
# define MANAGER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <iterator>
# include <Clients.hpp>
# include <colors.h>

class Channel;
class Sockets;

//Static management
class Manager
{
	private:
		static std::string 			_hostname;
		static std::vector<Clients>	_clients;
		static std::vector<Channel>	_channel;
	public:
		static	std::vector<std::string> _chanoper; // changed the name to chanop > more accurate. Public cuse no need to be private and helps.

		// static	void		setChanop( void ); //TODO: change name to setChanCommand
		static	int			addClient(int id);
		static	void		removeClient(int id);
		// static	void		addChannel(std::string name) = 0;//TODO: gabi
		// static	void		removeChannel(std::string name) = 0; //TODO: Gabi
		// static	Channel&	getChannelByName(std::string name) = 0; //TODO: Gabi
		// static	Client&		getClientById(int id) = 0; //TODO:

		//Channel commands
		// // KICK - Eject a client from the channel
		// static	kick(Channel &channel, int userId) = 0;//TODO:
		// // INVITE - Invite a client to a channel
		// static	invite(Channel &channel, int userId) = 0; //TODO:
		// // TOPIC - Change or view the channel topic
		// static	topic(Channel &channel, int option, int userId) = 0; //TODO:
		// // MODE - Change the channel’s mode:
		// static	mode(Channel &channel, int option, int userId) = 0; //TODO:

		static	std::vector<Clients>::iterator	getClientById(int id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	void	parseCommands(std::vector<Clients>::iterator iter, char *buffer, int read);
		static	void	checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
};

#endif
