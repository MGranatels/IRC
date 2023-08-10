#include <Manager.hpp>
#include <Channel.hpp>

std::vector<Clients> 		Manager::_clients;
std::vector<Channel> 		Manager::_channels;
std::string 				Manager::hostname = "localhost";
std::map<std::string, ActionFunction> Manager::actionMap;
std::map<std::string, ModeFunction> Manager::modeMap;



void Manager::on(std::string event, ActionFunction fun) {
	actionMap.insert(std::pair<std::string, ActionFunction>(event, fun));
}

void Manager::defineActionMap( void )
{
	on("JOIN", &Manager::joinAction);
	on("QUIT", &Manager::quitAction);
	on("KICK", &Manager::kickAction);
	on("PART", &Manager::partAction);
	on("MODE", &Manager::modeAction);
	on("TOPIC", &Manager::topicAction);
	on("JOIN", &Manager::joinAction);
	on("INVITE", &Manager::inviteAction);
	on("PRIVMSG", &Manager::privAction);
	on("LIST", &Manager::listAction);
	on("NAMES", &Manager::namesAction);
	on("WHO", &Manager::whoAction);
	on("LUSERS", &Manager::lusersAction);
}

bool	Manager::checkClientData( Clients& foundClient )
{
	std::vector<std::string> cmd = foundClient.getCmd();
	if (foundClient.getClientSettings() == true)
		return true;
	for (unsigned int i = 0 ; i < cmd.size(); i++)
	{
		if (cmd[i] == "PASS")
			foundClient.setPassword(cmd[i + 1]);
		else if (cmd[i] == "NICK")
			foundClient.setNickname(cmd[i + 1]);
		else if (cmd[i] == "USER")
			foundClient.setUsername(cmd[i + 1]);
	}
	return false;
}

int	Manager::runChanActions(Clients& client) {
	std::vector<std::string> cmd = client.getCmd();
	std::string actionName = cmd[0];

	// for (unsigned int i = 0; i < cmd.size(); i++)
	// 	std::cout << i << " cmd: " <<  cmd[i] << std::endl;
	defineActionMap();
	if (!isValidClient(client.getId()))
		return -1;
	// Find the action function in the map
	std::map<std::string, ActionFunction>::iterator it = actionMap.find(actionName);
	if (it != actionMap.end())
		(it->second)(client);
	else
		sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + " " + actionName + ": Unknown command", client.getId());
	return -1;
}

void	Manager::partAction( Clients &client )
{
	std::vector<std::string> cmd = client.getCmd();
	std::string partMessage = "";
	if (cmd.size() > 2)
		partMessage = cmd[2];
	// Find the channel
	Channel& channel = Manager::getChannelByName(cmd[1]);
	// Check if the client is in the channel
	if (!channel.isClientInChannel(client.getId()))
		return ;
	// Notify other clients in the channel about the PART
	if (!partMessage.empty())
		BroadcastMessageChan(channel, formatMessage(client, "PART_CHANNEL") + " " + channel.getName() + " " + partMessage);
	else
		BroadcastMessageChan(channel, formatMessage(client, "PART_CHANNEL") + " " + channel.getName());
	// Send a PART message to the client to indicate they left the channel.
	sendIrcMessage(formatMessage(client) + " PART " + channel.getName(), client.getId());
	BroadcastMessageChan(channel, formatMessage(client, "QUIT_CHANNEL") + " :has quit");
	// Remove the user from the channel
	channel.removeClient(client.getId());
	messageUpdateUserList(channel, client);
	return ;
}

void	Manager::joinAction( Clients &client )
{
	// First, check if the channel exists
	std::vector<std::string> cmd = client.getCmd();
	std::string &channelName = cmd[1];
	int isValidChan = isValidChannel(channelName);

	std::cout << "Check Nick in Client Vector " << client.getNickname() << std::endl ;
	if (isValidChan == CREATED)
	{
		if (!checkChannelParameters(channelName, client, cmd))
			return ;
		Channel& existingChannel = getChannelByName(channelName);
		existingChannel.addClient(client.getId());
		joinProtocol(client, existingChannel, client.getId());
	}
	else if (isValidChan == VALID_NAME)
	{
		_channels.push_back(Channel(channelName));
		_channels.back().addClient(client.getId());
		_channels.back().addOperator(client.getId());
		joinProtocol(client, _channels.back(), client.getId());
	}
	else if (isValidChan == NOT_VALID)
		sendIrcMessage(formatMessage(client, BADCHANNELNAME) + " " + channelName + " :Invalid channel name, try with #", client.getId());
	return ;
}

void	Manager::kickClientFromChannel(Clients& kicker, Clients& target, Channel& channel)
{
	std::cout << "entrou para kickar" << std::endl;
	std::cout <<  channel.isClientInChannel(target.getId()) << std::endl;
	if (channel.isClientInChannel(target.getId()))
	{
		// Notify the kicked client
		sendIrcMessage(formatMessage(kicker, CANNOTSENDTOCHAN) + ' ' + channel.getName() + ' ' + target.getNickname() + " :You have been removed from channel", kicker.getId());
		// Remove the user from the channel
		channel.removeClient(target.getId());
		messageUpdateUserList(channel, target);
		sendIrcMessage(formatMessage(target, NAMREPLY) + " = " + channel.getName() + " :" + getUsersList(channel), target.getId());
		sendIrcMessage(formatMessage(target, ENDOFNAMES) + " " + channel.getName() + " :End of NAMES list",  target.getId());
	}
}

void Manager::kickAction( Clients &kicker )
{
	std::vector<std::string> cmd = kicker.getCmd();
	std::vector<Clients>::iterator leaver = _clients.begin();
	int	flag = 0;
	if (cmd.size() > 3)
		flag = 1;
	// Find if channel created
	if (!isValidChannel(cmd[1]))
	{
		std::cout << "erro1" << std::endl;
		return;
	}
	Channel& channel = getChannelByName(cmd[1]);
	//Check if client exists
	for (; leaver <= _clients.end(); leaver++)
	{
		if (leaver == _clients.end())
		{
			std::cout << Red << "client you wanted to ban does not have that nickname." << NC << std::endl;
			return;
		}
		std::cout << "|" << ":" + leaver->getNickname() << "| vs |" << cmd[2] << "|" << std::endl;
		if ((":" + leaver->getNickname()) == cmd[2 + flag])
			break;
	}
	//Check if the kicker is an operator or has necessary permissions to kick
	if (!checkChannelOp(channel, kicker.getId()))
	{
		// Kicker is not allowed to kick others
		std::cout << "erro4" << std::endl;
		sendIrcMessage(formatMessage(kicker, "NOTICE") + " :You do not have permission to kick users from the channel", kicker.getId());
		return;
	}
	// Perform the kick
	kickClientFromChannel(kicker, *leaver, channel);
	return;
}


void	Manager::quitAction( Clients &client )
{
	removeClientFromAllChannels(client.getId());
	sendIrcMessage(formatMessage(client) + " QUIT :Goodbye!", client.getId());
	return ;
}

void	Manager::privAction( Clients &client)
{
	//TODO: remember later to Verify User Permissions
	std::vector<std::string> cmd = client.getCmd();
	std::string &recipient = cmd[1];
	std::vector<std::string> message;

	if (client.fullMessage.find(':') != std::string::npos)
		message = split(client.fullMessage, ":");
	else
	{
		sendIrcMessage(formatMessage(client, ERR_NOTEXTTOSEND) + " :No text to send, TRY ADDING A ':' AFTER THE NICKNAME", client.getId());
		return ;
	}
	if (isValidChannel(recipient) == CREATED)
	{
		Channel& channel = getChannelByName(recipient);
		std::cout  << channel.isClientMuted(client.getId()) << std::endl;
		if (channel.isClientMuted(client.getId()))
		{
			sendIrcMessage(formatMessage(client, CANNOTSENDTOCHAN) + " " + recipient + " :Cannot send message to channel, you have been Muted, shiuuuuuuu!", client.getId());
			return ;
		}
		if (!channel.isClientInChannel(client.getId()))
		{
			sendIrcMessage(formatMessage(client, CANNOTSENDTOCHAN) + " " + recipient + " :Cannot send message to channel, you are not part of it pathetic fool", client.getId());
			return ;
		}
		BroadcastMessageChan(client.getId(), channel, formatMessage(client) + " PRIVMSG " + recipient + " " + message[1]);
	}
	else if (isValidClient(recipient))
	{
		int recipientId = getClientByNick(recipient).getId();
		if (recipientId != client.getId())
			sendIrcMessage(formatMessage(client) + " PRIVMSG " + recipient + " " + message[1], recipientId);
	}
}

void	Manager::modeAction( Clients& client )
{
	if (!validateMode(client))
		return ;
	changeMode(client);
	return ;
}

void	Manager::topicAction( Clients &client )
{
	std::vector<std::string> cmd = client.getCmd();
	Channel& _channel = getChannelByName(cmd[1]);
	if (cmd.size() < 3 && _channel.getTopic().empty()) {
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set", client.getId());
		return ;
	}
	if (cmd.size() < 3) {
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), client.getId());
		return ;
	}
	if (!_channel.isClientOperator(client.getId())) {
		sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, you're not channel operator.", client.getId());
		return ;
	}
	//Check if user has permissions in channel
	if (!_channel.isModeSet("t")) {
		sendIrcMessage(formatMessage(client, CHANOPRIVSNEEDED) + " :Permission denied, topic Channel 't' not set.", client.getId());
		return ;
	}
	_channel.setTopic(split(client.fullMessage, ":")[1]);
	BroadcastMessageChan(_channel, formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic());
}

void	Manager::inviteAction( Clients& inviter )
{
	std::vector<std::string> cmd = inviter.getCmd();
	std::string &channelName = cmd[2];
	std::string	&invitedClient = cmd[1];
	int isValidChan = isValidChannel(channelName);
	if (isValidChan == CREATED)
	{
		Channel &channel = getChannelByName(channelName);
		if (!channel.isClientOperator(inviter.getId())) {
			sendIrcMessage(formatMessage(inviter, CHANOPRIVSNEEDED) + " " + channelName + " :Permission denied, you're not channel operator.", inviter.getId());
			return ;
		}
		if (!channel.isClientInChannel(inviter.getId())) {
			sendIrcMessage(formatMessage(inviter, ERR_USERONCHANNEL) + " " + channelName + " :User not in Channel", inviter.getId());
			return ;
		}
		if (!isValidClient(invitedClient)) {
			sendIrcMessage(formatMessage(inviter, NOSUCHNICK) + " " + invitedClient + " :Not such user in Server", inviter.getId());
			return ;
		}
		if (channel.isClientInChannel(getClientByNick(invitedClient).getId())) {
			sendIrcMessage(formatMessage(inviter, ERR_USERONCHANNEL) + " " + invitedClient + " :user already in Channel " + channelName, inviter.getId());
			return ;
		}
		channel.addInvitee(getClientByNick(invitedClient).getId());
		sendIrcMessage(formatMessage(inviter, INVITING) + " " + invitedClient + " " + channelName, inviter.getId());
		sendIrcMessage(formatMessage(inviter) + " NOTICE " + invitedClient + " you have been invited to join " + channelName, getClientByNick(invitedClient).getId());
	}
	else
		sendIrcMessage(formatMessage(inviter, ERR_NOSUCHCHANNEL) + " " + channelName + " :Not such channel in Server ",  inviter.getId());
}

void	Manager::nickAction( Clients& client )
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendIrcMessage(formatMessage(client, NONICKNAMEGIVEN) + " :No nickname given", client.getId());
		return ;
	}
	if (cmd[1].size() > 9)
	{
		sendIrcMessage(formatMessage(client, ERRONEUSNICKNAME) + " " + cmd[1] + " :Erroneous nickname", client.getId());
		return ;
	}
	if (isValidClient(cmd[1]))
	{
		sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + cmd[1] + " :Nickname is already in use", client.getId());
		return ;
	}
	client.setNickname(cmd[1]);
	sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + client.getNickname() + cmd[1] + " :Nickname changed successfully", client.getId());
}

void	Manager::whoAction( Clients &client )
{
	if (client.getCmd().size() == 1)
	{
		sendWhoMessage(getAllClientsIds(), "*", client);
		std::cout << "Give info about everyone in server" << std::endl;
	}
	else if (client.getCmd().size() <= 3 && isValidChannel(client.getCmd()[1]) == CREATED)
	{
		Channel &channel = getChannelByName(client.getCmd()[1]);
		if (client.getCmd().size() == 3 && client.getCmd()[2] == "o")
			sendWhoMessage(channel.getOperators(), channel.getName(), client);
		else
			sendWhoMessage(channel.getClients(), channel.getName(), client);
		std::cout << "Give info about a channel " + client.getCmd()[1] << std::endl;
	}
	else
		sendIrcMessage(formatMessage(client, UNKNOWNCOMMAND) + ": USAGE: WHO [<mask> [<o>]]", client.getId());
}

void	Manager::listAction( Clients& client)
{
	std::vector<std::string> cmd = client.getCmd();
	std::string listMessage;

	std::cout << cmd.size() << std::endl;
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel& channel = *it;
		std::string numberClients = channel.getClientsCountStr();
		listMessage += formatMessage(client, RPL_LIST) + " " + channel.getName() + " " + numberClients + " :" + channel.getTopic() + "\r\n";
		sendIrcMessage(listMessage, client.getId());
	}
	sendIrcMessage(formatMessage(client, RPL_LISTEND) + " :End of LIST", client.getId());
}

void	Manager::namesAction( Clients& client )
{
	std::vector<std::string> cmd = client.getCmd();

	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " NAMES :Not enough parameters", client.getId());
		return ;
	}
	if (isValidChannel(cmd[1]) == CREATED) {
		Channel& channel = getChannelByName(cmd[1]);
		std::string usersList = getUsersList(channel);
		sendIrcMessage(formatMessage(client, NAMREPLY) + " = " + channel.getName() + " :" + usersList, client.getId());
		sendIrcMessage(formatMessage(client, ENDOFNAMES) + " " + channel.getName() + " :End of NAMES list", client.getId());
	}
	else
		sendIrcMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + cmd[1] + " :No such channel", client.getId());
}

void	Manager::lusersAction( Clients& client )
{
	std::stringstream ss;
	ss << _clients.size();
	std::string numberClients = ss.str();
	std::string numberOpps = getAllChanOps();
	std::string unkownClients = getUnkownClients();
	ss.str("");
	ss.clear();
	ss << _channels.size();
	std::string numberChannels = ss.str();
	std::cout << "Number of channels: " <<  _channels.size() << std::endl;
	sendIrcMessage(formatMessage(client, LUSERCLIENT) + " :There are " + numberClients + " users" + " 1 services on 1 server(s)", client.getId());
	sendIrcMessage(formatMessage(client, LUSEROP) + " :" + numberOpps + " operator(s) online", client.getId());
	sendIrcMessage(formatMessage(client, LUSERUNKNOWN) + " :" + unkownClients + " unknown connection(s)", client.getId());
	sendIrcMessage(formatMessage(client, LUSERCHANNELS) + " :" + numberChannels + " channels formed", client.getId());
}
