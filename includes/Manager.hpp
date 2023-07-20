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

	public:
		static	std::vector<std::string> _chanoper; // changed the name to chanop > more accurate. Public cuse no need to be private and helps.
		
		static	void		setChanop( void );
		static	void		addClient(int id);
		static	void		removeClient(int id);
		static	std::vector<Clients>::iterator	getClientById(int id);
		static	const std::vector<Clients>&	getClients();
		static	int		firstTimeClient(std::vector<Clients>::iterator it);
		static	void	parseCommands(std::vector<Clients>::iterator iter, char *buffer, int read);
		static	void	checkCommands(std::vector<std::string> splits, std::vector<Clients>::iterator iter);
};

#endif