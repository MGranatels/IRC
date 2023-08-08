#include <Manager.hpp>
#include <utils.hpp>

int	Manager::tOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		_channel.setMode("t");
		_channel.setTopic(split[3]);
		sendIrcMessage(formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :" + _channel.getTopic(), _client.getId());
	}
	else {
		_channel.unsetMode("t");
		_channel.setTopic("");
		BroadcastMessageChan(_channel, formatMessage(_client, TOPIC_CHANNEL) + " " + _channel.getName() + " :No topic is set");
	}
	return 1;
}

int	Manager::iOperator(std::vector<std::string> split, Channel& _channel)
{
	if (split[2][0] == '+')
		_channel.setMode("i");
	else
		_channel.unsetMode("i");
	return 1;
}

int	Manager::kOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		_channel.setPassword(split[3]);
		_channel.setMode("k");
	}
	else {
		_channel.setPassword("");
		_channel.unsetMode("k");
	}
	return 1;
}

int	Manager::oOperator(std::vector<std::string> split, Channel& channel, Clients& _client)
{
	if (split.size() != 4)
		return (sendIrcMessage(formatMessage(channel, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
	if (_client.getNickname() == split[3])
		return (sendIrcMessage(formatMessage(_client, CHANOPRIVSNEEDED) + " :Permission denied, can't remove your own privileges.", _client.getId()));
	if (!isValidClient(split[3]))
		return (sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId()));
	if (split[2][0] == '+') {
		channel.addOperator(getClientByNick(split[3]).getId());
		channel.setMode("o");
	}
	else
	{
		channel.removeOperator(getClientByNick(split[3]).getId());
		channel.unsetMode("o");
	}
	messageUpdateUserList(channel, _client);
	return 1;
}

int	Manager::lOperator(std::vector<std::string> split, Channel& _channel, Clients& _client)
{
	if (split[2][0] == '+') {
		if (split.size() != 4)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		if (!IsDigit(split[3]))
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Argument type for Selected Mode. Type HELP For a List of Commands", _client.getId()));
		if (std::atoi(split[3].c_str()) <= 0)
			return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Limit Needs to be a Value Higher than 0", _client.getId()));
		_channel.setMode("l");
		_channel.setLimit(std::atoi(split[3].c_str()));
	}
	else {
		_channel.unsetMode("l");
		_channel.setLimit(0);
	}
	return 1;
}

int Manager::mOperator(std::vector<std::string> split, Channel& _channel, Clients _client)
{
	if (split.size() != 4)
		return (sendIrcMessage(formatMessage(_client, NEEDMOREPARAMS) + " :Incorrect Number os Arguments for Selected Mode. Type HELP For a List of Commands", _client.getId()));
	if (!isValidClient(split[3]))
		return (sendIrcMessage(formatMessage(_client, NOSUCHNICK) + " :No such Nickname", _client.getId()));
	Clients& foundClient = getClientByNick(split[3]);
	if (!_channel.isClientInChannel(foundClient.getId()))
		return (sendIrcMessage(formatMessage(_client, ERR_USERONCHANNEL) + " " + _channel.getName() + " :User not in Channel", _client.getId()));
	if (split[2][0] == '+')
	{
		_channel.addMuted(foundClient.getId());
		_channel.setMode("m");
	}
	else
	{
		_channel.removeMuted(foundClient.getId());
		_channel.unsetMode("m");
	}
	return 1;
}
