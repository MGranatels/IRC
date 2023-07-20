#ifndef	CLIENTS_HPP
# define CLIENTS_HPP

# include <Sockets.hpp>

class Clients
{
	private:
		int			_id;
		std::string	_password;
		std::string	_nickcname;
		std::string	_username;

	public:
		
		Clients( int _id);
		~Clients( void );
		//Setters 
		void		setId( int id );
		void		setNickname( std::string nickname );
		void		setUsername( std::string username );
		void		setPassword( std::string username );
		//Getters
		int			getId( void ) const;
		std::string	getNickname( void ) const;
		std::string	getUsername( void ) const;
		std::string	getPassword( void ) const;
		void		initClient(std::string nickname, std::string username);
};

#endif