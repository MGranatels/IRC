#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <iterator>
# include <Clients.hpp>
# include <colors.h>
# include <Channel.hpp>

class Bot
{
	private:
		std::string	botName;
	public:
		//Function declarations
		Bot( Clients& client );
		Bot( void );
		~Bot( void );

		void	helpAction(Clients &client);
		void	helpMap(Clients &client);
		void	welcomeMessage( Clients &client );
		void	setBotName( std::string name );
		void	tellJoke( Clients& client );
		void	actionCenter( Clients &client);
		int		botIrcMessage(std::string message, int clientId);
		const std::string botFormatMessage( void );
		std::string	getBotName( void ) const;
		std::vector<std::string> vecToLowerCase(const std::vector<std::string> &vec);
		std::string	toLowerCase( const std::string &str);
		const std::string	botFormatMessage(const Clients &client);
};

#endif
