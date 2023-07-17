#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <Sockets.hpp>
# include <Clients.hpp>

class Channel
{
	private:
		std::string _topic;
        std::vector<int> _clientsIds;
        std::vector<int> _ChannelOpsIds;

    public:
        std::string getTopic() const;
        int BroadcastMessage(std::string message, int sender_fd);
};

#endif