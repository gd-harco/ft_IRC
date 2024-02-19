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
        std::string userToInvite = args[1];
        std::string channelName = args[2].substr(1);
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw NotAChannel();
        }
        Channel *channel = _channels.find(channelName)->second;
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
        if (channel->IsInChannel(userToInvite))
        {
            NumericReplies::Error::userOnChannel(*client, userToInvite, channelName);
            client->updateClientStatus(_epollFd);
            throw AlreadyOnChannel();
        }
        Client *clientInvited = findClient(userToInvite);
        if (clientInvited != NULL)
        {
            NumericReplies::reply::inviting(*client, userToInvite, channelName);
            client->updateClientStatus(_epollFd);
            NumericReplies::Notification::inviteNotify(*clientInvited, client->GetNickname(), channelName);
            clientInvited->updateClientStatus(_epollFd);
        }
        else
        {
            // TODO exception serveur ?
            std::cerr << "Je connais pas : " << userToInvite << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}