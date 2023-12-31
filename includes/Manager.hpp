#ifndef  MANAGER_HPP
# define MANAGER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <iterator>
# include <Clients.hpp>
# include <colors.h>
# include <Channel.hpp>
# include <Bot.hpp>


class Sockets;

typedef void (*ActionFunction)(Clients& client);
typedef void (*ModeFunction)(Channel& channel, Clients& client);

//Static management
class Manager
{
	private:
		static std::vector<Clients>	_clients;
		static std::vector<Channel>	_channels;
   		static std::map<std::string, ActionFunction> actionMap;
   		static std::map<std::string, ModeFunction> modeMap;
		static std::string			_serverName;
	public:
		static std::string 			hostname;
		static	int					_awake;
		/* Channels Methods*/
		static	int			addClient(int id);
		static	void		removeClient(int id);

		// static	void		removeChannel(std::string name); //TO DO: Gabi
		static	Channel&	getChannelByName(std::string name);
		static int			isValidChannel(std::string name);
		static std::string 		getUsersList(Channel &Channel);
		static	void	startBot( Clients& client );

		// --------- Channel Actions ---------
		static	void 		on(std::string event, void (*ActionFunction)(Clients& client));
		static	void		joinAction( Clients &client );
		static	void		partAction( Clients &client);
		static	void		modeAction( Clients &client );
		static	void		quitAction( Clients &client );
		static	void		topicAction( Clients &client );
		static	void		inviteAction( Clients &client );
		static	void		privAction( Clients &client );
		static	void		nickAction( Clients &client );
		static	void		listAction( Clients& client );
		static	void		namesAction( Clients& client );
		static	void		whoAction( Clients &client );
		static	void		kickAction( Clients &client );
		static	void		kickClientFromChannel(Clients& kicker, Clients& target, Channel& channel, std::string kickReason);
		static	void		lusersAction( Clients& client );

		//static	int			(*muteAction)( Clients &client);
		static	void		joinProtocol(Clients &client, Channel &channelName, int clientId);
		static	void		sendWhoMessage(const std::vector<int> &clientsIds, const std::string &chanName, Clients &sender);
		static	int			runChanActions( Clients& client);
		static	int			sendIrcMessage(std::string message, int id);
		static	void		BroadcastMessageChan(Channel &channel, std::string message);
		static	void		BroadcastMessageChan(int senderId, Channel &channel, std::string message);
		static	void			messageUpdateUserList(Channel &channel, Clients &client);
		static const std::string	formatMessage(const Clients &client);
		static const std::string	formatMessage(const Clients &client, std::string code);
		static const std::string	formatMessage(const Channel &_channel, std::string code);
		static	void		setChannOpps(Clients& client);
		static	int			validateMode( Clients client );
		static	bool		checkFlagFormat(std::string flag);
		static	void		changeMode( Clients &client );
		static	void		kOperator(Channel& _channel, Clients& _client);
		static	void		oOperator(Channel& _channel, Clients& _client);
		static	void		lOperator(Channel& _channel, Clients& _client);
		static	void		iOperator(Channel& _channel, Clients& _client);
		static	void		tOperator(Channel& _channel, Clients& _client);
		static	void		mOperator(Channel& _channel, Clients& _client);
		static	void		bOperator(Channel& _channel, Clients& _client);
		static	void		sOperator(Channel& _channel, Clients& _client);
		static	void		onMode(std::string event, ModeFunction fun);
		static	bool		checkChannelPassword(Clients client, std::string channelName, std::string key);
		static	bool		checkChannelLimit(std::string channelName, Clients client);
		static	bool		checkChannelInvite(std::string channelName, Clients client);
		static	bool		checkChannelParameters(Clients client, std::string channelName, std::string key);
		static	bool		checkChannelBan(std::string channelName, Clients client);
		static	bool		checkChannelOp(Channel _channel, int id);
		static	void		defineActionMap( void );
		static	void		defineModeMap( void );
		static	std::map<std::string, std::string>	getChannelNameAndKey(std::vector<std::string>& cmd);
		static	void	leaveAllChannels(Clients& client);

		/* Client Methods*/
		static	std::vector<Clients>::iterator	getClientById(int id);
		static	Clients&	getClientByNick(std::string nickname);
		static	bool		isValidClient(std::string nickname);
		static	bool		isValidClient(int	id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	bool	checkClientData( Clients& foundClient );
		static  bool	checkNickName(Clients client);
		static	bool	checkPassword(Clients client, std::string password);
		static	void	checkSuperUser(std::string channelName, Clients client);
		static	void	removeClientFromAllChannels(int clientId);
		static	std::vector<int> getAllClientsIds(void);
		static	std::string	getAllChanOps( void );
		static	std::string	getUnkownClients( void );
};

#endif
