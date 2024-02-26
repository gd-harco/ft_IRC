#include "server.hpp"
#include "channel.hpp"

void Server::kick(vectorCommand args, Client *client)
{
	try
	{
        if (args.size() < 3)
        {
            NumericReplies::Error::needMoreParams(*client, args[0]);
            client->updateClientStatus(_epollFd);
            throw NeedMoreParams();
        }
        std::string channelName = args[1].substr(1);
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw NotAChannel();
        }
        Channel *channel= _channels.find(channelName)->second;
        if (!channel->IsInChannel(client->GetNickname()))
        {
            NumericReplies::Error::notOnChannel(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw NotInTheChannel();
        }
        if (!channel->isOp(client->GetNickname()))
        {
            NumericReplies::Error::chanOpPrivsNeeded(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw OperatorIsNeeded();
        }
		if (!channel->IsInChannel(args[2]))
		{
			NumericReplies::Error::userNotInChannel(*client, args[2], channelName);
			client->updateClientStatus(_epollFd);
			throw NotInTheChannel();
		}
		else
		{
			stringClientMap	ClientChannelMap = channel->GetClients();
			for (stringClientMap::iterator it = ClientChannelMap.begin(); it != ClientChannelMap.end(); it++)
			{
				fdClientMap::iterator	findIt = _clients.find(it->second);
				if (findIt != _clients.end())
				{
					Client	*userToSend = findIt->second;
					NumericReplies::Notification::kickNotify(*userToSend, args[2], channelName, args[args.size() - 1]);
					userToSend->updateClientStatus(_epollFd);
				}
			}
				channel->RemoveClient(args[2]);
		}
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
