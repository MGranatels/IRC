#include <Manager.hpp>
#include <utils.hpp>

void	Manager::tOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd[2][0] == '+') {
		_channel.setMode("t");
	}
	else {
		_channel.unsetMode("t");
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
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
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
		sendIrcMessage(formatMessage(channel, NEEDMOREPARAMS) + " " + channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
	else if (_client.getNickname() == cmd[3])
		sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " " + channel.getName() + " :Permission denied, can't remove your own privileges.", _client.getId());
	else if (!isValidClient(cmd[3]))
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " " + channel.getName() + " :No such Nickname", _client.getId());
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
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		else if (!IsDigit(cmd[3]))
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Argument type for Selected Mode. Type HELP For a List of Commands", _client.getId());
		else if (std::atoi(cmd[3].c_str()) <= 0)
			sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Limit Needs to be a Value Higher than 0", _client.getId());
		else {
			_channel.setMode("l");
			_channel.setLimit(std::atoi(cmd[3].c_str()));
		}
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
		sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		return ;
	}
	if (!isValidClient(cmd[3])) {
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " " + _channel.getName() + " :No such Nickname", _client.getId());
		return ;
	}
	Clients& foundClient = getClientByNick(cmd[3]);
	if (!_channel.isClientInChannel(foundClient.getId())) {
		sendIrcMessage(formatMessage(_client, ERR_USERONCHANNEL) + " " + _channel.getName()  + " " + _channel.getName() + " :User not in Channel", _client.getId());
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
	std::string	banReason;

	if (cmd.size() != 4) {
		sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		return ;
	}
	if (!isValidClient(cmd[3])) {
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " " + _channel.getName() + " :No such Nickname", _client.getId());
		return ;
	}
	Clients& foundClient = getClientByNick(cmd[3]);
	// Check If user is channel operator
	if (_channel.isClientOperator(foundClient.getId())) {
		sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " " + _channel.getName() + " :Invalid action for selected user. A channel Operator can't be banned", _client.getId());
		return ;
	}
	if (cmd[2][0] == '+') {
		_channel.addBanned(foundClient, _client);
		_channel.setMode("b");
		banReason = "Banned from channel. ";
		kickClientFromChannel(_client, foundClient, _channel, banReason);
	}
	else {
		_channel.removeBanned(foundClient.getId());
		_channel.unsetMode("b");
		sendIrcMessage(formatMessage(_client) + " MODE " + _channel.getName() + " -b " + foundClient.getNickname() +  "!*@*", foundClient.getId());
	}
	return ;
}

//Lets create a new operator, its s and its for a SuperUser mode

void	Manager::sOperator(Channel& _channel, Clients& _client)
{
	std::vector<std::string> cmd = _client.getCmd();

	if (cmd.size() != 4) {
		sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " " + _channel.getName() + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId());
		return ;
	}
	if (!isValidClient(cmd[3])) {
		sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " " + _channel.getName() + " :No such Nickname", _client.getId());
		return ;
	}
	Clients& foundClient = getClientByNick(cmd[3]);
	if (cmd[2][0] == '+') {
		_channel.addSuperUser(foundClient.getNickname());
		//inform user about super mode activated
		_channel.setMode("s");
		sendIrcMessage(formatMessage(_channel, CHANNELMODEIS) + " " + _channel.getName() + " :+s SuperUser Activated", foundClient.getId());
	}
	else {
		_channel.removeSuperUser();
		_channel.unsetMode("s");
		sendIrcMessage(formatMessage(_channel, CHANNELMODEIS) + " " + _channel.getName() + " :-s SuperUser Deactivated", foundClient.getId());
	}
	return ;
}
