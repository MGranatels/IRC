#include <Manager.hpp>
#include <Channel.hpp>

void Manager::on(std::string event, ActionFunction fun) {
	actionMap.insert(std::pair<std::string, ActionFunction>(event, fun));
}

void Manager::defineActionMap( void )
{
	//on("FieryBot", &Manager::botpAction);
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
	// for (unsigned int i = 0; i < cmd.size(); i++)
	// 	std::cout << i << " cmd: " <<  cmd[i] << std::endl;
	client.removeCmd();
	if (cmd[0].empty())
		return 1;
	std::string actionName = cmd[0];

	if (cmd.size() >= 4 && cmd[2] == "WHO")
		return -1;
	defineActionMap();
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
	if (cmd.size() == 1) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return;
	}
	if (cmd.size() > 2)
		for (unsigned int i = 2; i < cmd.size(); i++)
			partMessage += cmd[i];
	Channel& channel = Manager::getChannelByName(cmd[1]);
	if (!channel.isClientInChannel(client.getId()))
		return ;
	BroadcastMessageChan(channel, formatMessage(client, "PART") + " " + channel.getName() + " " + partMessage);
	sendIrcMessage(formatMessage(client) + " PART " + channel.getName(), client.getId());
	BroadcastMessageChan(channel, formatMessage(client, "QUIT") + " :has quit the channel");
	channel.removeClient(client.getId());
	if (channel.isClientOperator(client.getId()))
		if (channel.getOperators().size() == 1)
		{
			channel.removeOperator(client.getId());
			channel.addOperator(channel.getClients()[0]);
		}
	messageUpdateUserList(channel, client);
	return ;
}

void	Manager::joinAction( Clients &client )
{
	std::vector<std::string> cmd = client.getCmd();
	std::map<std::string, std::string> channelProp = getChannelNameAndKey(cmd);
	std::map<std::string, std::string>::iterator it = channelProp.begin();
	for (std::map<std::string, std::string>::const_iterator it = channelProp.begin(); it != channelProp.end(); ++it) {
		std::cout << "Channel: " << it->first << " | Key: " << it->second << std::endl;
	}
	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return;
	}
	if (cmd[1] == "0") {
		leaveAllChannels(client);
		return ;
	}
	while (it != channelProp.end())
	{
		int isValidChan = isValidChannel(it->first);
		if (isValidChan == CREATED)
		{
			Channel& existingChannel = getChannelByName(it->first);
			if (existingChannel.isClientInChannel(client.getId()))
				return ;
			if (!checkChannelParameters(client, it->first, it->second)) {
				it++;
				continue ;
			}
			existingChannel.addClient(client.getId());
			joinProtocol(client, existingChannel, client.getId());
		}
		else if (isValidChan == VALID_NAME)
		{
			_channels.push_back(Channel(it->first));
			_channels.back().addClient(client.getId());
			_channels.back().addOperator(client.getId());
			joinProtocol(client, _channels.back(), client.getId());
		}
		else if (isValidChan == NOT_VALID)
			sendIrcMessage(formatMessage(client, BADCHANNELNAME) + " " + it->first + " :Invalid channel name, try with #", client.getId());
		it++;
	}
}

void	Manager::kickClientFromChannel(Clients& kicker, Clients& target, Channel& channel, std::string kickReason)
{
	std::cout << "entrou para kickar" << std::endl;
	// Get commands and check if there are any extra arguments:
	if (channel.isClientInChannel(target.getId()))
	{
		if (kickReason.empty())
			BroadcastMessageChan(channel, ":" + kicker.getNickname() + " KICK " + channel.getName() + " " + target.getNickname() + " :No reason Given.");
		else
			BroadcastMessageChan(channel, ":" + kicker.getNickname() + " KICK " + channel.getName() + " " + target.getNickname() + " :" + kickReason);
		// BroadcastMessageChan(channel, formatMessage(kicker, "KICK") + " " + channel.getName() + " " + target.getNickname()  + " :has been removed from channel");
		channel.removeClient(target.getId());
		messageUpdateUserList(channel, target);
	}
}

void Manager::kickAction( Clients &kicker )
{
	std::vector<std::string> cmd = kicker.getCmd();
	std::string	kickReason;
	// Find if channel created
	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(kicker, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", kicker.getId());
		return;
	}
	if (!isValidChannel(cmd[1]) || cmd.size() < 3) {
		sendIrcMessage(formatMessage(kicker, ERR_NOSUCHCHANNEL) + " " + cmd[1] + " :No such channel exists argument was not given", kicker.getId());
		return;
	}
	Channel& channel = getChannelByName(cmd[1]);
	if (!isValidClient(cmd[2])) {
		sendIrcMessage(formatMessage(kicker, NOSUCHNICK) + " " + cmd[2] + " :No such user with that Nick", kicker.getId());
		return;
	}
	Clients& leaver = getClientByNick(cmd[2]);
	if (!checkChannelOp(channel, kicker.getId())) {
		sendIrcMessage(formatMessage(kicker, CHANOPRIVSNEEDED) + " " + channel.getName() + " :You do not have permission to kick users from the channel", kicker.getId());
		return;
	}
	if (!channel.isClientInChannel(leaver.getId())) {
			sendIrcMessage(formatMessage(kicker, USERNOTINCHANNEL) + " " + channel.getName() + " :" +  leaver.getNickname() + " Is not on this channel" , kicker.getId());
			return ;
	}
	if (leaver.getNickname() == kicker.getNickname()) {
		sendIrcMessage(formatMessage(kicker, NOPRIVILEGES) + " " +  channel.getName() + " :You cannot kick yourself", kicker.getId());
		return ;
	}
	// Perform the kick
	for (unsigned int i = 3; i < cmd.size(); i++)
		kickReason += cmd[i] + " ";
	kickClientFromChannel(kicker, leaver, channel, kickReason);
	return;
}


void	Manager::quitAction( Clients &client )
{
	removeClientFromAllChannels(client.getId());
	sendIrcMessage(formatMessage(client) + " QUIT :Goodbye!", client.getId());
	return ;
}

void	Manager::startBot( Clients& client )
{
	std::vector<std::string> cmd = client.getCmd();

	Bot mybot;

	if (isInside(mybot.vecToLowerCase(cmd), "wake"))
	{
		mybot.botIrcMessage(mybot.botFormatMessage() + client.getNickname() + "Yawns(...) Oh, hello there! I must have been taking a short nap.", client.getId());
		mybot.botIrcMessage(mybot.botFormatMessage() + client.getNickname() + "I'm wide awake now and ready to assist you. How can I help you today?", client.getId());
		_awake = 1;
	}

	else if (_awake == 1)
		mybot.actionCenter(client);
}

void	Manager::privAction( Clients &client)
{
	//TODO: remember later to Verify User Permissions
	std::vector<std::string> cmd = client.getCmd();
	std::string &recipient = cmd[1];
	std::vector<std::string> message;

	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return;
	}
	if (client.fullMessage.find(':') != std::string::npos)
		message = split(client.fullMessage, ":");
	if (cmd[1] == "FieryBot") {
		startBot(client);
		return ;
	}
	if (isValidChannel(recipient) == CREATED)
	{
		Channel& channel = getChannelByName(recipient);
		std::cout  << channel.isClientMuted(client.getId()) << std::endl;
		if (channel.isClientMuted(client.getId())) {
			sendIrcMessage(formatMessage(client, CANNOTSENDTOCHAN) + " " + recipient + " :Cannot send message to channel, you have been Muted, shiuuuuuuu!", client.getId());
			return ;
		}
		BroadcastMessageChan(client.getId(), channel, formatMessage(client) + " PRIVMSG " + recipient + " " + message[1]);
	}
	else if (isValidClient(recipient)) {
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
	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return;
	}
	if (!isValidChannel(cmd[1])) {
		sendIrcMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + cmd[1] + " :No such channel exists", client.getId());
		return ;
	}
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
	_channel.setTopic(removeCharacter(split(client.fullMessage, ":")[1], '\n'));
	BroadcastMessageChan(_channel, formatMessage(client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic());
}

void	Manager::inviteAction( Clients& inviter )
{
	std::vector<std::string> cmd = inviter.getCmd();
	std::string &channelName = cmd[2];
	std::string	&invitedClient = cmd[1];
	int isValidChan = isValidChannel(channelName);
	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(inviter, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", inviter.getId());
		return ;
	}
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
	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
		return ;
	}
	if (cmd[1].size() > 9) {
		sendIrcMessage(formatMessage(client, ERRONEUSNICKNAME) + " " + cmd[1] + " :Erroneous nickname", client.getId());
		return ;
	}
	if (isValidClient(cmd[1])) {
		sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + cmd[1] + " :Nickname is already in use", client.getId());
		return ;
	}
	client.setNickname(cmd[1]);
	sendIrcMessage(formatMessage(client, NICKNAMEINUSE) + " " + client.getNickname() + cmd[1] + " :Nickname changed successfully", client.getId());
}

void	Manager::whoAction( Clients &client )
{
	if (client.getCmd().size() == 1) {
		sendWhoMessage(getAllClientsIds(), "*", client);
		std::cout << "Give info about everyone in server" << std::endl;
	}
	else if (client.getCmd().size() <= 3 && isValidChannel(client.getCmd()[1]) == CREATED) {
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

void Manager::listAction(Clients& client)
{
    std::vector<std::string> cmd = client.getCmd();

    // Send RPL_LISTSTART numeric reply
    sendIrcMessage(formatMessage(client, RPL_LISTSTART) + " :Channel list", client.getId());
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        Channel& channel = *it;
        std::string numberClients = channel.getClientsCountStr();
        // Send RPL_LIST numeric reply for each channel
        sendIrcMessage(formatMessage(client, RPL_LIST) + " " + channel.getName() + " " + numberClients + " :" + channel.getTopic(), client.getId());
    }

    // Send RPL_LISTEND numeric reply
    sendIrcMessage(formatMessage(client, RPL_LISTEND) + " :End of LIST", client.getId());
}

void	Manager::namesAction( Clients& client )
{
	std::vector<std::string> cmd = client.getCmd();

	if (cmd.size() < 2) {
		sendIrcMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR :Not enough parameters", client.getId());
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
