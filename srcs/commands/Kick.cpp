#include "server.hpp"
#include "channel.hpp"

void Server::kick(vectorCommand args, Client *client)
{
	try
	{
        if (args.size() < 3)
        {
            NumericReplies::Error::needMoreParams(*client, args[0]);
            throw NeedMoreParams();
        }
        std::string channelName = args[1].substr(1);
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            throw NotAChannel();
        }
        Channel *channel= _channels.find(channelName)->second;
        if (!channel->IsInChannel(client->GetNickname()))
        {
            NumericReplies::Error::notOnChannel(*client, channelName);
            throw NotInTheChannel();
        }
        if (!channel->isOp(client->GetNickname()))
        {
            NumericReplies::Error::chanOpPrivsNeeded(*client, channelName);
            throw OperatorIsNeeded();
        }
        for (size_t i = 2; i < args.size() - 1; i++)
        {
            if (!channel->IsInChannel(args[i]))
            {
                NumericReplies::Error::userNotInChannel(*client, args[i], channelName);
                throw NotInTheChannel();
            }
            else
            {
                Client clientKicked = channel->GetClients().find(args[i])->second;
                NumericReplies::Notification::kickNotify(clientKicked, client->GetNickname(), channelName, args[args.size() - 1]);
                channel->RemoveClient(args[i]);
            }
        }
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
