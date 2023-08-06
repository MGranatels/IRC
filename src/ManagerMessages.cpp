#include <Manager.hpp>
#include <Channel.hpp>

const std::string Manager::formatMessage(const Clients &client) {
	return (":" + client.getNickname() + "!" + client.getUsername() + "@" + Manager::hostname);
}

const std::string Manager::formatMessage(const Clients &client, std::string code)
{
	return (":" + hostname + " " + code + " " + client.getNickname());
}

const std::string Manager::formatMessage(const Channel &_channel, std::string code)
{
	return (":" + hostname + " " + code + " " + _channel.getName());
}

void Manager::BroadcastMessageChan(Channel &channel, std::string message)
{
	std::vector<int> users;

	users = channel.getClients();
	std::vector<int>::iterator it = users.begin();
	for (; it != users.end(); it++)
		sendIrcMessage(message, *it);
}

void Manager::BroadcastMessageChan(int senderid, Channel &channel, std::string message)
{
	std::vector<int> users;

	users = channel.getClientsNoSender(senderid);
	std::vector<int>::iterator it = users.begin();
	for (; it != users.end(); it++)
		sendIrcMessage(message, *it);
}

void Manager::messageUpdateUserList(Channel &channel, Clients &client)
{
	BroadcastMessageChan(channel, formatMessage(client, NAMREPLY) + " = " + channel.getName() + " :" + getUsersList(channel));
	BroadcastMessageChan(channel, formatMessage(client, ENDOFNAMES) + " " + channel.getName() + " :End of NAMES list");
}

void	Manager::joinProtocol(Clients &client, Channel &channel, int &clientId)
{
	// Send the JOIN message to the client
	sendIrcMessage(formatMessage(client) + " JOIN " + channel.getName(), clientId);
	//:<server_hostname> 332 <user_nickname> <channel_name> :<channel_topic>
	if (channel.getTopic().empty())
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + channel.getName() + " :No topic is set", clientId);
	else
		sendIrcMessage(formatMessage(client, TOPIC_CHANNEL) + " " + channel.getName() + " :" + channel.getTopic(), clientId);
	messageUpdateUserList(channel, client);
}
