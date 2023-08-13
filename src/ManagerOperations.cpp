#include <Manager.hpp>
#include <utils.hpp>

void	Manager::tOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd[2][0] == '+') {
		_channel.setMode("t");
		_channel.setTopic(cmd[3]);
		sendIrcMessage(formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), _client.getId());
	}
	else {
		_channel.unsetMode("t");
		_channel.setTopic("");
		BroadcastMessageChan(_channel, formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set");
	}
}

void	Manager::iOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd[2][0] == '+')
		_channel.setMode("i");
	else
		_channel.unsetMode("i");
}

void	Manager::kOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd[2][0] == '+') {
		if (cmd.size() != 4) {
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
			return ;
		}
		_channel.setPassword(cmd[3]);
		_channel.setMode("k");
	}
	else {
		_channel.setPassword("");
		_channel.unsetMode("k");
	}
}

void	Manager::oOperator(Channel& channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd.size() != 4)
		sendIrcMessage(formatMessage(channel, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
	else if (_client.getNickname() == cmd[3])
		sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " :Permission denied, can't remove your own privileges.", _client.getId());
	else if (!isValidClient(cmd[3]))
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId());
	else if (cmd[2][0] == '+') {
		channel.addOperator(getClientByNick(cmd[3]).getId());
		channel.setMode("o");
		messageUpdateUserList(channel, _client);
	}
	else {
		channel.removeOperator(getClientByNick(cmd[3]).getId());
		channel.unsetMode("o");
		messageUpdateUserList(channel, _client);
	}
}

void	Manager::lOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd[2][0] == '+') {
		if (cmd.size() != 4)
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		else if (!IsDigit(cmd[3]))
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Argument type for Selected Mode. Type HELP For a List of Commands", _client.getId());
		else if (std::atoi(cmd[3].c_str()) <= 0)
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Limit Needs to be a Value Higher than 0", _client.getId());
		_channel.setMode("l");
		_channel.setLimit(std::atoi(cmd[3].c_str()));
	}
	else {
		_channel.unsetMode("l");
		_channel.setLimit(0);
	}
	return ;
}

void	Manager::mOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd.size() != 4) {
		sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		return ;
	}
	if (!isValidClient(cmd[3])) {
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId());
		return ;
	}
	Clients& foundClient = getClientByNick(cmd[3]);
	if (!_channel.isClientInChannel(foundClient.getId())) {
		sendIrcMessage(formatMessage(_client, ERR_USERONCHANNEL) + " " + _channel.getName() + " :User not in Channel", _client.getId());
		return ;
	}
	if (cmd[2][0] == '+') {
		_channel.addMuted(foundClient.getId());
		_channel.setMode("m");
	}
	else {
		_channel.removeMuted(foundClient.getId());
		_channel.unsetMode("m");
	}
	return ;
}

void	Manager::bOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd.size() != 4) {
		sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		return ;
	}
	if (!isValidClient(cmd[3])) {
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId());
		return ;
	}
	Clients& foundClient = getClientByNick(cmd[3]);
	// Check If user is channel operator
	if (_channel.isClientOperator(foundClient.getId())) {
		sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " " + _channel.getName() + " :Invalid action for selected user. A channel Operator can't be banned", _client.getId());
		return ;
	}
	if (cmd[2][0] == '+') {
		_channel.addBanned(foundClient.getId());
		_channel.setMode("b");
		kickClientFromChannel(_client, foundClient, _channel);
		_channel.removeClient(foundClient.getId());
		sendIrcMessage(formatMessage(_client, BANNEDFROMCHAN) + " " + _channel.getName(), foundClient.getId());
		_channel.removeClient(foundClient.getId());
	}
	else {
		std::cout << "Unban this guy: " << foundClient.getNickname() << std::endl;
		_channel.removeBanned(foundClient.getId());
		_channel.unsetMode("b");
	}
	return ;
}