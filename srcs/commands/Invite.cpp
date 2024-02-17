#include "server.hpp"

void Server::invite(vectorCommand args, Client *client)
{
    try
    {
        if (args.size() < 3)
        {
            NumericReplies::Error::needMoreParams(*client, args[0]);
            throw NeedMoreParams();
        }
        std::string channelName = args[1].substr(1);
        std::string userToInvite = args[2];
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw NotAChannel();
        }
        Channel *channel = _channels.find(channelName)->second;
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}