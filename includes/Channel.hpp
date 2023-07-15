#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>

class	Channel
{
	private:
		std::string _name;
		std::string _topic;

	public:
		Channel( std::string name);
		~Channel( void );

};

#endif