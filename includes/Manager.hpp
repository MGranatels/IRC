#ifndef  MANAGER_HPP
# define MANAGER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <iterator>
# include <Clients.hpp>

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
		typedef int (*MemberFunctionPointer)();
		static	std::map<std::string, MemberFunctionPointer> _chanActions;

		static	void		addClient(int id);
		static	void		removeClient(int id);

		// --------- Channel Actions (assim esta bom Gabi?) ---------
		static	void		setChanActions( void ); //TO DO: change name to setChanCommand
		static	int			kickAction( void );
		static	int			modeAction( void );
		static	int			topicAction( void );
		static	int			inviteAction( void );


		//static	kick(Channel &channel, int userId) = 0;//TODO:
		//static	invite(Channel &channel, int userId) = 0; //TODO:
		//static	mode(Channel &channel, int option, int userId) = 0; //TODO:
		//static	topic(Channel &channel, int option, int userId) = 0; //TODO:

		//static	void		addChannel(std::string name);//TO DO: gabi
		//static	void		removeChannel(std::string name); //TO DO: Gabi
		//static	Channel&	getChannelByName(std::string name); //TO DO: Gabi
		//static	Client&		getClientById(int id); //TO DO:


		static	std::vector<Clients>::iterator	getClientById(int id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	int		parseActions(std::vector<Clients>::iterator iter, char *buffer, int read);
		static	void	checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
};
#endif

		//Channel commands
		// KICK - Eject a client from the channel
		// INVITE - Invite a client to a channel
		// TOPIC - Change or view the channel topic
		// MODE - Change the channel’s mode: