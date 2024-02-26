#include "server.hpp"

void Server::ping(vectorCommand args, Client *client)
{
    if (args.size() < 2)
    {
        NumericReplies::Error::needMoreParams(*client, args[0]);
        client->updateClientStatus(_epollFd);
        throw NeedMoreParams();
    }
    std::string token = args[1];
    NumericReplies::Notification::pongNotify(*client, NETWORK_NAME, token);
    client->updateClientStatus(_epollFd);
}