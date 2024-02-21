#include "server.hpp"

void    Server::quit(vectorCommand args, Client *client)
{
    if (args.size() == 1)
    {
        NumericReplies::Error::needMoreParams(*client, args[0]);
        client->updateClientStatus(_epollFd);
        throw NeedMoreParams();
    }
    client->addMessageToSendbox("ERROR :Closing connection: " + args[1] + DELIMITER);
    client->updateClientStatus(_epollFd);
    // TODO : comprendre pourquoi ca fonctionne pas de faire ca
//    RemoveClient(client->GetFd());
//    deleteClient(_clients.find(client->GetFd()));
}
