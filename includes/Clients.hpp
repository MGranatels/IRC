#ifndef	CLIENTS_HPP
# define CLIENTS_HPP

# include <Sockets.hpp>

class Clients
{
	private:
		int			_id;
		bool		_opChannel;
		bool		_clientSettings;
		std::string	_password;
		std::string	_nickcname;
		std::string	_username;
		std::vector<std::string>	_cmd;


	public:

		std::string	fullMessage;
		Clients( int _id);
		~Clients( void );
		//Setters
		void		initClient(std::string nickname, std::string username);
		void		setCmd( std::vector<std::string> cmd );
		void		setId( int id );
		void		setNickname( std::string nickname);
		void		setUsername( std::string username );
		void		setPassword( std::string username );
		void		setClientSettings( bool clientSettings );
		void		setOppChannel( bool opChannel );
		//Getters
		int							getId( void ) const;
		std::string					getNickname( void ) const;
		std::string					getUsername( void ) const;
		std::string					getPassword( void ) const;
		std::vector<std::string>	getCmd( void ) const;
		bool						getClientSettings( void ) const;
		bool						getOppChannel( void ) const;
};

#endif
