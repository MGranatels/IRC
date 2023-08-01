#ifndef  MANAGER_HPP
# define MANAGER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <iterator>
# include <Clients.hpp>
# include <colors.h>
# include <Channel.hpp>


class Sockets;

//Static management
class Manager
{
	private:
		static std::vector<Clients>	_clients;
		static std::vector<Channel>	_channels;
	public:

		static std::string 			_hostname;
		static	int			addClient(int id);
		static	void		removeClient(int id);
		// static	void		removeChannel(std::string name); //TO DO: Gabi
		static	Channel&	getChannelByName(std::string name);
		static int			isValidChannel(std::string name);
		static std::string 		getUsersList(Channel &Channel);

		// --------- Channel Actions (assim esta bom Gabi?) ---------
		static	int			joiningChannel(Clients &client, std::string channelName, int clientId);
		static	int			joinAction( std::string channelName, int clientId );
		static void			joinProtocol(Clients &client, Channel &channelName, int &clientId);
		static	int			kickAction( void );
		static	int			modeAction( void );
		static	int			topicAction( void );
		static	int			inviteAction( std::string nickName, int clientId );
		static	int			privAction( void );
		static	int			runChanActions( std::vector<std::string> splits, int clientId);
		static	void		sendIrcMessage(std::string message, int id);
		static	void		BroadcastMessageChan(Channel &channel, std::string message);
		static	void		setChannOpps(Clients *client);
		static const std::string	formatMessage(Clients &client);
		static const std::string	formatMessage(Clients &client, std::string code);

		static	std::vector<Clients>::iterator	getClientById(int id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	bool	checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
		static  bool	checkNickName(Clients client);
		static	bool	checkPassword(Clients client, std::string password);
};
#endif

		//Channel commands
		// KICK - Eject a client from the channel
		// INVITE - Invite a client to a channel
		// TOPIC - Change or view the channel topic
		// MODE - Change the channel’s mode:
