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
		static std::string 			hostname;

		/* Channels Methods*/
		static	int			addClient(int id);
		static	void		removeClient(int id);

		// static	void		removeChannel(std::string name); //TO DO: Gabi
		static	Channel&	getChannelByName(std::string name);
		static int			isValidChannel(std::string name);
		static std::string 		getUsersList(Channel &Channel);

		// --------- Channel Actions ---------
		static	int			joinAction( std::string channelName, int clientId );
		static void			joinProtocol(Clients &client, Channel &channelName, int &clientId);
		static	int			kickAction( void );
		static	int			modeAction( std::vector<std::string> split, int clientId );
		static	int			topicAction( void );
		static	int			privAction( const Clients &client, std::vector<std::string> splits );
		static	int			inviteAction( std::string nickName, int clientId );
		static	int			runChanActions( std::vector<std::string> splits, int clientId);
		static	int			sendIrcMessage(std::string message, int id);
		static	void		BroadcastMessageChan(Channel &channel, std::string message);
		static	void		BroadcastMessageChan(int senderId, Channel &channel, std::string message);
		static const std::string	formatMessage(const Clients &client);
		static const std::string	formatMessage(const Clients &client, std::string code);
		static	void		setChannOpps(Clients *client);
		static	bool		checkChannelOp(Channel _channel, int id);
		static	int			validateMode(std::vector<std::string> split, Clients client);

		/* Client Methods*/
		static	std::vector<Clients>::iterator	getClientById(int id);
		static	Clients&	getClientByNick(std::string nickname);
		static	bool		isValidClient(std::string nickname);
		static	bool		isValidClient(int	id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	bool	checkClientData(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
		static  bool	checkNickName(Clients client);
		static	bool	checkPassword(Clients client, std::string password);
		static	void	removeClientFromAllChannels(int clientId); //TODO:
};
#endif
