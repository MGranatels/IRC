#ifndef Error_HPP
# define Error_HPP


# define WELCOME_MESSAGE	"001"
# define CHANNEL_OPPS		"005"
# define CHANNELMODEIS		"324"
# define TOPIC_CHANNEL		"332"
# define INVITING			"341"
# define NAMREPLY			"353"
# define ENDOFNAMES			"366"
# define NOSUCHNICK			"401"
# define ERRONEUSNICKNAME	"432"
# define NICKNAMEINUSE		"433"
# define UNKNOWNCOMMAND		"421"
# define NOTONCHANNEL		"442"
# define NEEDMOREPARAMS		"461"
# define PASSWDMISMATCH		"464"
# define CHANNELISFULL		"471"
# define INVITEONLYCHAN 	"473"
# define BADCHANNELKEY		"475"
# define CHANOPRIVSNEEDED	"482"
# define UMODEUNKNOWNFLAG	"501"
# define BADCHANNELNAME		"476"
# define ERR_USERONCHANNEL	"443"
# define ERR_NOSUCHCHANNEL	"403"

class Error
{
	public:
		static int message(std::string error)
		{
			std::cerr << error << std::endl;
			return (0);
		}
};

#endif
