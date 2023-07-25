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

		static	int			addClient(int id);
		static	void		removeClient(int id);
		// static	void		addChannel(std::string name);//TO DO: gabi
		// static	void		removeChannel(std::string name); //TO DO: Gabi
		// static	Channel&	getChannelByName(std::string name); //TO DO: Gabi
		// static	Client&		getClientById(int id); //TO DO:

		// --------- Channel Actions (assim esta bom Gabi?) ---------
		static	int			joinAction( void );
		static	int			kickAction( void );
		static	int			modeAction( void );
		static	int			topicAction( void );
		static	int			inviteAction( void );
		static	int			privAction( void );
		static	int			runChanActions( std::vector<std::string> splits );
		static	void		sendIrcMessage(std::string message, int id);



		static	std::vector<Clients>::iterator	getClientById(int id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	bool	checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
		static  bool	checkNickName(int id, std::string nickName);
};
#endif

		//Channel commands
		// KICK - Eject a client from the channel
		// INVITE - Invite a client to a channel
		// TOPIC - Change or view the channel topic
		// MODE - Change the channel’s mode:
