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

typedef void (*ActionFunction)(Clients& client, std::vector<std::string>& splits, std::string fullMessage);

//Static management
class Manager
{
	private:
		static std::vector<Clients>	_clients;
		static std::vector<Channel>	_channels;
   		static std::map<std::string, ActionFunction> actionMap;

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
		static	void 		on(std::string event, void (*ActionFunction)(Clients& client, std::vector<std::string>& splits, std::string fullMessage));
		static	void		joinAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		quitAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		partAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		modeAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage  );
		static	void		topicAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		inviteAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		privAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		nickAction( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static	void		kickAction( void );
		//static	int			(*muteAction)( Clients &client, std::vector<std::string> &splits, std::string fullMessage );
		static void			joinProtocol(Clients &client, Channel &channelName, int clientId);
		static	int			runChanActions( std::vector<std::string> &splits, int clientId, std::string full_message);
		static	int			sendIrcMessage(std::string message, int id);
		static	void		BroadcastMessageChan(Channel &channel, std::string message);
		static	void		BroadcastMessageChan(int senderId, Channel &channel, std::string message);
		static	void			messageUpdateUserList(Channel &channel, Clients &client);
		static const std::string	formatMessage(const Clients &client);
		static const std::string	formatMessage(const Clients &client, std::string code);
		static const std::string	formatMessage(const Channel &_channel, std::string code);
		static	void		setChannOpps(Clients *client);
		static	bool		checkChannelOp(Channel _channel, int id);
		static	int			validateMode(std::vector<std::string> split, Clients client);
		static	bool		checkFlagFormat(std::string flag);
		static	int			changeMode(std::vector<std::string> split, Clients &client);
		static	int			kOperator(std::vector<std::string> split, Channel& _channel, Clients& _client);
		static	int			oOperator(std::vector<std::string> split, Channel& _channel, Clients& _client);
		static	int			lOperator(std::vector<std::string> split, Channel& _channel, Clients& _client);
		static	int			iOperator(std::vector<std::string> split, Channel& _channel);
		static	int			tOperator(std::vector<std::string> split, Channel& _channel, Clients& _client);
		static	int			mOperator(std::vector<std::string> split, Channel& _channel, Clients _client);
		static	bool		checkChannelPassword(std::string channelName, Clients client, std::vector<std::string> &splits);
		static	bool		checkChannelLimit(std::string channelName, Clients client);
		static	bool		checkChannelInvite(std::string channelName, Clients client);
		static	bool		checkChannelParameters(std::string channelName, Clients client, std::vector<std::string> &splits);
		static	void		defineActionMap();


		/* Client Methods*/
		static	std::vector<Clients>::iterator	getClientById(int id);
		static	Clients&	getClientByNick(std::string nickname);
		static	bool		isValidClient(std::string nickname);
		static	bool		isValidClient(int	id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	bool	checkClientData(std::vector<std::string> &splits, std::vector<Clients>::iterator iter);
		static  bool	checkNickName(Clients client);
		static	bool	checkPassword(Clients client, std::string password);
		static	void	removeClientFromAllChannels(int clientId); //TODO:
};

#endif
