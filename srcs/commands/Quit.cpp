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
    /*
    for (fdClientMap::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        int clientFd = it->first;
        if (clientFd == client->GetFd()) {
            deleteClient(it);
            return;
        }
    }
    */
}
