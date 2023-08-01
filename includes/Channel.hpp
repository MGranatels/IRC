#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <algorithm>
# include <utils.hpp>
# include <Clients.hpp>

//notes:
// Channel names are case insensitive.

#define NOT_VALID -1
#define VALID_NAME 0
#define CREATED 1

class	Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::vector<int> _clientsIds;
		std::vector<int> _operatorsIds;

	public:
		Channel(std::string name);
		Channel(std::string name, std::string topic);
		~Channel( void );
		std::vector<int> getClientsNoSender(int senderId) const;
		const std::vector<int>& getClients(void) const;
		void addClient(int newClientId);
		void removeClient(int clientId);
		void setTopic(std::string topic);
		void setName(std::string name);
		std::string getTopic(void) const;
		std::string getName(void) const;
		const std::vector<int>& getOperators(void) const;
		void addOperator(int operatorId);
		void removeOperator(int operatorId);
		bool isClientIn(int clientId);
		// TODO: bool isChanop(int ClientId); //to do -> return if a given client is a chanop i.e has right to execute the commands
};

#endif
