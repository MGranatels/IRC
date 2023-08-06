#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <algorithm>
# include <utils.hpp>
# include <Clients.hpp>
# include <map>
# include <functional>

//notes:
// Channel names are case insensitive.

#define NOT_VALID -1
#define VALID_NAME 0
#define CREATED 1

enum ChannelModeStatus {
    MODE_NOT_SET = 0,
    MODE_SET = 1
};

class	Channel
{
	private:
		std::map<std::string, ChannelModeStatus> _modes;
		std::string _name;
		std::string _topic;
		std::string _password;
		std::vector<int> _clientsIds;
		std::vector<int> _operatorsIds;
		std::vector<int> _inviteesIds;
		unsigned int	 _limit;

	public:
		Channel(std::string name);
		Channel(std::string name, std::string topic);
		~Channel( void );

		// Channel Operators:
		void	addOperator(int operatorId);
		void	removeOperator(int operatorId);
		void	addClient(int newClientId);
		void	removeClient(int clientId);
		void	addInvitee(int clientId);
		void	removeInvitee(int clientId);
		bool	isClientInvited(int clientId);
		bool	isModeSet(const std::string& mode);
		bool	isClientInChannel(int clientId);
		bool	isClientOperator( int clientId);

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
		// TODO: bool isChanop(int ClientId); //to do -> return if a given client is a chanop i.e has right to execute the commands
};

#endif
