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
		std::vector<int> _usersIds;
		std::vector<int> _operatorsIds;

	public:
		Channel(std::string name);
		Channel(std::string name, std::string topic);
		~Channel( void );
		std::vector<int> getUsersNoSender(int senderId) const;
		const std::vector<int>& getUsers(void) const;
		void addUser(int newUserId);
		void removeUser(int UserId);
		void setTopic(std::string topic);
		void setName(std::string name);
		std::string getTopic(void) const;
		std::string getName(void) const;
		const std::vector<int>& getOperators(void) const;
		void addOperator(int operatorId);
		void removeOperator(int operatorId);
		bool isChanop(int userId); //to do -> return if a given user is a chanop i.e has right to execute the commands
};

#endif
