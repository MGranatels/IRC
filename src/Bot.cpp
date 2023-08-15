#include <Bot.hpp>
# include <utils.hpp>

Bot::Bot( Clients &client ) : botName("FieryBot") {
	std::string message = "🌙 Hello there! I'm your friendly bot FieryBot, and I'm currently taking a little nap. Just type 'Wake up!' to get my attention and I'll be wide awake, ready to assist you when requested. Remember, all you need to do is say 'Wake up!' and I'll be at your service.🌟" ;

	botIrcMessage(botFormatMessage() + " PRIVMSG "  + message, client.getId());
	// Send ircMessage to new user in channel
	//botIrcMessage("Hi, I am your bot, happy to help, happy to serve :)", client.getId());
	//sendIrcMessage(formatMessage(client) + " PRIVMSG " + recipient + " " + message[1], recipientId);

}

Bot::Bot( void ) : botName("FieryBot") {
}

Bot::~Bot( void ) {
}

const std::string Bot::botFormatMessage( void ) {
	return (":" + this->botName + "!" + this->botName + "@" + "localhost PRIVMSG ");
}

std::string	Bot::getBotName( void ) const {
	return (botName);
}

void	Bot::setBotName( std::string name ) {
	botName = name;
}

int		Bot::botIrcMessage(std::string message, int clientId)
{
	message = message + "\r\n";
	std::cout << "Sending message: " << message << std::endl;
	if (send(clientId, message.c_str(), message.length(), 0) == -1)
		exit(Error::message("Error sending message"));
	return 0;
}

void	Bot::helpMap( Clients& client )
{
	botIrcMessage(botFormatMessage() + client.getNickname() + " List of available commands:", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /QUIT	- Quit IRC.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /JOIN	- Join a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /KICK	- Kick a user from a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /WHO	- Displays user information based on a given criteria or mask.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /KICK	- Removes a user from a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /PART	- Leaves one or multiple channels.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /MODE	- Changes or displays modes for users or channels.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /LIST	- Lists available channels and their topics.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /JOIN	- Joins one or multiple channels.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /TOPIC	- Changes or displays the topic of a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /INVITE	- Invites a user to a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /PRIVM	- Sends a private message to another user or a channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /NAMES	- Lists all users in a given channel.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " /LUSER	- Displays server statistics and user counts.", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " For more details on a command, type /HELP <command>.", client.getId());
}

void	Bot::helpAction( Clients& client )
{
	std::vector<std::string> cmd = vecToLowerCase(client.getCmd());
	cmd.erase(cmd.begin());
	
	if (isInside(cmd, "join"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /JOIN <channel_name> - Join the specified channel by providing its name.", client.getId());
	else if (isInside(cmd, "quit"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /QUIT - Leave the IRC server. No additional parameters needed.", client.getId());
	else if (isInside(cmd, "kick"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /KICK <channel_name> <user> - Kick the specified user from the given channel.", client.getId());
	else if (isInside(cmd, "who"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /WHO <mask> - Query users based on a given criteria or mask. The mask can be optional.", client.getId());
	else if (isInside(cmd, "part"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /PART <channel_name> - Leave the specified channel. You can also specify multiple channels separated by commas.", client.getId());
	else if (isInside(cmd, "mode"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /MODE <channel/user> <modes> - Change or display modes for the specified user or channel.", client.getId());
	else if (isInside(cmd, "list"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /LIST [<channel_name>] - List available channels and their topics. Providing a channel name is optional.", client.getId());
	else if (isInside(cmd, "topic"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /TOPIC <channel_name> [new_topic] - View or set the topic for the specified channel. Providing a new topic is optional.", client.getId());
	else if (isInside(cmd, "invite"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /INVITE <user> <channel_name> - Invite the specified user to the given channel.", client.getId());
	else if (isInside(cmd, "privm"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /PRIVM <user/channel> <message> - Send a private message to the specified user or channel.", client.getId());
	else if (isInside(cmd, "names"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /NAMES <channel_name> - List all users in the specified channel.", client.getId());
	else if (isInside(cmd, "luser"))
		botIrcMessage(botFormatMessage() + client.getNickname() + " /LUSER - Get statistics about the server and its users. No additional parameters needed.", client.getId());
	else
		botIrcMessage(botFormatMessage() + client.getNickname() + "It seems my wires got a bit crossed there. Could you kindly provide more information so I can assist you accurately? .", client.getId());
}

void	Bot::tellJoke( Clients& client ) {
	botIrcMessage(botFormatMessage() + client.getNickname() + " Why do programmers prefer using the dark mode?", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " Because light attracts bugs!", client.getId());
}

void	Bot::welcomeMessage( Clients &client )
{
	botIrcMessage(botFormatMessage() + client.getNickname() + " Hello, " + client.getNickname() + "!", client.getId());
	botIrcMessage(botFormatMessage() + client.getNickname() + " Welcome to the IRC server. I am your friendly bot her to help :D", client.getId());
}


void	Bot::actionCenter( Clients &client )
{
	std::vector<std::string> cmd = vecToLowerCase(client.getCmd());

	if (isInside(cmd, "help"))
		helpMap(client);
	else if (isInside(cmd, "hi") || isInside(cmd, "hello"))
		welcomeMessage(client);
	else if (isInside(cmd, "joke"))
		tellJoke(client);
	else
		helpAction(client);
}

std::string Bot::toLowerCase( const std::string &str) {
    std::string result = str;
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

std::vector<std::string> Bot::vecToLowerCase(const std::vector<std::string> &vec) {
    std::vector<std::string> result;
    for (size_t i = 0; i < vec.size(); i++) {
        result.push_back(toLowerCase(vec[i]));
    }
    return result;
}