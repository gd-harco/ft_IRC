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
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}