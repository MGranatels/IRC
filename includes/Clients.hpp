#ifndef	CLIENTS_HPP
# define CLIENTS_HPP

# include <sockets.hpp>

class Clients
{
	private:
		int			_id;
		std::string	_nickcname;
		std::string	_username;

	public:
		Clients( int _id);
		~Clients( void );
		//Setters 
		static std::vector<Clients *> _clients;
		void		setId( int id );
		void		setNickname( std::string nickname );
		void		setUsername( std::string username );
		//Getters
		int			getId( void ) const;
		std::string	getNickname( void ) const;
		std::string	getUsername( void ) const;
		Clients*	getClientById(int id);
		void	addClient(Clients *client);
		static	void	removeClient(int id);
};

#endif