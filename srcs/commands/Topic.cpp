#include "server.hpp"

void Server::topic(vectorCommand args, Client *client)
{
    if (args.size() == 1 || args[1].empty())
    {
        NumericReplies::Error::needMoreParams(*client, "TOPIC");
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
    if (args.size() == 2 || (args.size() == 3 && args[2].empty()))
    {
        if (channel->GetTopics().empty())
        {
            NumericReplies::reply::noTopic(*client, channelName);
            client->updateClientStatus(_epollFd);
        }
        else
        {
            NumericReplies::reply::topic(*client, channelName, channel->GetTopics());
            client->updateClientStatus(_epollFd);
        }
        return;
    }
    if (channel->GetRTopic() && !channel->isOp(client->GetNickname()))
    {
        NumericReplies::Error::chanOpPrivsNeeded(*client, channelName);
        client->updateClientStatus(_epollFd);
        throw OperatorIsNeeded();
    }
    channel->SetTopics(args[2]);
    stringClientMap   clientsMap = channel->GetClients();
    for (stringClientMap::iterator clientMapIt = clientsMap.begin(); clientMapIt != clientsMap.end(); clientMapIt++)
    {
        if (_clients.find(clientMapIt->second) != _clients.end())
        {
            Client  *clientToSend = _clients.find(clientMapIt->second)->second;
            NumericReplies::reply::topic(*clientToSend, channelName, channel->GetTopics());
            clientToSend->updateClientStatus(_epollFd);
        }
    }
}