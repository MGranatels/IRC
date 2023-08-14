#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <algorithm>
# include <utils.hpp>
# include <Clients.hpp>
# include <map>
# include <functional>
#include <ctime>

//notes:
// Channel names are case insensitive.

#define NOT_VALID -1
#define VALID_NAME 0
#define CREATED 1

enum ChannelModeStatus {
    MODE_NOT_SET = 0,
    MODE_SET = 1
};

typedef struct s_banClient {
	std::string banMask; //mask to identify user
	std::string timestamp;
	std::string	banSetter; //The nickname of the user who set the ban.
} t_banClient;

class	Channel
{
	private:
		std::map<std::string, ChannelModeStatus> _modes;
		std::string _name;
		std::string _topic;
		std::string _password;
		std::string	_superUser;
		std::vector<int> _clientsIds;
		std::vector<int> _operatorsIds;
		std::vector<int> _inviteesIds;
		std::vector<int> _mutedIds;
		std::map<int, t_banClient> _bannedIds;
		unsigned int	 _limit;

	public:
		Channel(std::string name);
		Channel(std::string name, std::string topic);
		~Channel( void );

		// Channel Operators:
		void	addOperator(int operatorId);
		void	addBanned(Clients &newClientId, Clients &BanSetterId);
		void	addClient(int newClientId);
		void	addInvitee(int clientId);
		void	addMuted(int newClientId);
		void	addSuperUser( std::string user);
		void	removeClient(int clientId);
		void	removeAllOperators( void );
		void	removeOperator(int operatorId);
		void	removeInvitee(int clientId);
		void	removeMuted(int clientId);
		void	removeBanned(int clientId);
		void	removeSuperUser( void );
		bool	isClientInvited(int clientId);
		bool	isModeSet(const std::string& mode);
		bool	isClientInChannel(int clientId);
		bool	isClientOperator( int clientId);
		bool 	isClientMuted( int clientId);
		bool	isClientBanned( int clientId);

		//Getters and Setters:
		void	setTopic(std::string topic);
		void	setName(std::string name);
		void	setMode(const std::string& mode);
		void	setPassword(std::string password);
		void	setLimit(int limit);
		void	unsetMode(const std::string& mode);

		std::map<std::string, ChannelModeStatus> getModes() const;
		std::vector<int> getClientsNoSender(int senderId) const;
		const std::vector<int>&	getClients(void) const;
		const std::vector<int>& getOperators(void) const;
		std::string		getTopic(void) const;
		std::string		getName(void) const;
		std::string		getPassword(void) const;
		unsigned int	getLimit(void) const;
		int				getClientsCount(void) const;
		std::string		getClientsCountStr(void) const;
		std::string		getSuperUser(void) const;
		std::string		getChannelModes( void );
		// TODO: bool isChanop(int ClientId); //to do -> return if a given client is a chanop i.e has right to execute the commands
};

#endif
