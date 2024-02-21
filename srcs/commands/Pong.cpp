#include "server.hpp"

void Server::pong(vectorCommand args, Client *client)
{
    if (args.size() < 2)
    {
        NumericReplies::Error::needMoreParams(*client, args[0]);
        client->updateClientStatus(_epollFd);
        throw NeedMoreParams();
    }
    std::string token = args[1];
    if (token != client.GetPingPongToken())
    {
        // Meaby deconnect the client when this happen
        throw BadPongResponse();
    }
}