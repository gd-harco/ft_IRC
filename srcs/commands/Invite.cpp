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
<<<<<<< HEAD
        std::string userToInvite = args[1];
        std::string channelName = args[2].substr(1);
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            throw NotAChannel();
        }
        Channel *channel = _channels.find(channelName)->second;
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
        if (channel->IsInChannel(userToInvite))
        {
            NumericReplies::Error::userOnChannel(client, userToInvite, channelName);
            throw AlreadyOnChannel();
        }
        // TODO: No such nick/channel 401
        // TODO: envoyer un message a l'user invité
=======
        std::string channelName = args[1].substr(1);
        std::string userToInvite = args[2];
        if (_channels.find(channelName) == _channels.end())
        {
            NumericReplies::Error::noSuchChannel(*client, channelName);
            client->updateClientStatus(_epollFd);
            throw NotAChannel();
        }
        Channel *channel = _channels.find(channelName)->second;
        
>>>>>>> 99f2315e224476f09fde1b120652854c07c86bf9
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}