//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::join(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
		throw (NotAuthenticate());

	// if (args.size() != 1)
	// {
	// 	std::cout << "invalid number of args" << std::endl;
	// 	return (false);
	// }
	if (args[0].find("#") != 0)
	{
		client->addMessageToSendbox(":irc.localhost 471 " + client->GetUsername() + " #" + args[0] + " :Cannot join channel\r\n");
		client->updateClientStatus(_epollFd);
		throw(UnableToCreateChannel());
	}
	std::string RealNameChannel(args[0].substr(1));
	if (_channels.find(RealNameChannel) == _channels.end())
	{
		std::cout << client->GetUsername() << " create channel " << RealNameChannel << std::endl;
		Channel *NewChannel = new Channel(RealNameChannel);
		NewChannel->AddClient(client->GetUsername(), client->GetFd());
		AddChannel(RealNameChannel, NewChannel);
		client->addMessageToSendbox(RPL_JOIN(client->GetUsername(), RealNameChannel));
		client->addMessageToSendbox(RPL_NAMREPLY(client->GetUsername(), RealNameChannel, NewChannel->GetAllNickname()));
		client->addMessageToSendbox(RPL_ENDOFNAMES(client->GetUsername(), RealNameChannel));
		client->updateClientStatus(_epollFd);

		return ;
	}
	_channels[RealNameChannel]->AddClient(client->GetUsername(), client->GetFd());
	stringClientMap	ClientChannel = _channels[RealNameChannel]->GetClients();
	client->addMessageToSendbox(RPL_JOIN(client->GetUsername(), RealNameChannel));
	client->addMessageToSendbox(RPL_NAMREPLY(client->GetUsername(), RealNameChannel, _channels.find(RealNameChannel)->second->GetAllNickname()));
	client->addMessageToSendbox(RPL_ENDOFNAMES(client->GetUsername(), RealNameChannel));
	client->updateClientStatus(_epollFd);
}

