#ifndef Error_HPP
# define Error_HPP

# define WELCOME_MESSAGE	"001"
# define CHANNEL_OPPS		"005"
# define LUSERCLIENT		"251"
# define LUSEROP			"252"
# define LUSERUNKNOWN		"253"
# define LUSERCHANNELS		"254"
# define RPL_LISTSTART		"321"
# define RPL_LIST			"322"
# define RPL_LISTEND		"323"
# define CHANNELMODEIS		"324"
# define TOPIC_CHANNEL		"332"
# define INVITING			"341"
# define RPL_WHOREPLY		"352"
# define NAMREPLY			"353"
# define ENDOFNAMES			"366"
# define NOSUCHNICK			"401"
# define NOSUCHSERVER		"402"
# define ERR_NOSUCHCHANNEL	"403"
# define CANNOTSENDTOCHAN	"404"
# define ERR_NOTEXTTOSEND	"412"
# define UNKNOWNCOMMAND		"421"
# define NONICKNAMEGIVEN	"431"
# define ERRONEUSNICKNAME	"432"
# define NICKNAMEINUSE		"433"
# define NOTONCHANNEL		"442"
# define ERR_USERONCHANNEL	"443"
# define NEEDMOREPARAMS		"461"
# define PASSWDMISMATCH		"464"
# define CHANNELISFULL		"471"
# define INVITEONLYCHAN 	"473"
# define BANNEDFROMCHAN		"474"
# define BADCHANNELKEY		"475"
# define BADCHANNELNAME		"476"
# define CHANOPRIVSNEEDED	"482"
# define UMODEUNKNOWNFLAG	"501"
# define RPL_ENDOFWHO		"315"

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
