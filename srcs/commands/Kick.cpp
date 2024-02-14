#include "server.hpp"
#include "channel.hpp"

void Server::kick(vectorCommand args, Client *client)
{
    (void) client;
	try
	{
        if (args.size() < 3)
            NumericReplies::Error::needMoreParams(*client, args[0]);
        for (size_t i = 0; i < args.size(); i++)
            std::cout << "args[" << i << "]: " << args[i] << std::endl;
        std::string channelName = args[1].substr(1);
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
        }
        Channel *channel= _channels.find(channelName)->second;
        if (!channel->isOp(client->GetNickname()))
        {
            NumericReplies::Error::chanOpPrivsNeeded(*client, channelName);
        }
        if (!channel->IsInChannel(client->GetNickname()))
        {
            NumericReplies::Error::notOnChannel(*client, channelName);
        }
        for (size_t i = 2; i < args.size() - 1; i++)
        {
            if (!channel->IsInChannel(args[i]))
            {
                NumericReplies::Error::userNotInChannel(*client, args[i], channelName);
                NumericReplies::Error::bannedFromChan(*client, channelName);
            }
            else
                channel->RemoveClient(args[i]);
        }
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
