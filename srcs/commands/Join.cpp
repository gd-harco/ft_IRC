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
	if (args[1].find("#") != 0)
	{
		client->addMessageToSendbox(":irc.localhost 471 " + client->GetUsername() + " #" + args[1] + " :Cannot join channel\r\n");
		client->updateClientStatus(_epollFd);
		throw(UnableToCreateChannel());
	}
	std::string RealNameChannel(args[1].substr(1));
	if (_channels.find(RealNameChannel) == _channels.end())
	{
		std::cout << client->GetUsername() << " create channel " << RealNameChannel << std::endl;
		Channel *NewChannel = new Channel(RealNameChannel, client->GetNickname());
		NewChannel->AddClient(client->GetNickname(), client->GetFd());
		AddChannel(RealNameChannel, NewChannel);
//		client->addMessageToSendbox(RPL_JOIN(client->GetUsername(), RealNameChannel));
		NumericReplies::Notification::joinNotify(*client, RealNameChannel);
//		client->addMessageToSendbox(RPL_NAMREPLY(client->GetUsername(), RealNameChannel, NewChannel->GetAllNickname()));
		NumericReplies::reply::nameInChannel(*client, RealNameChannel, NewChannel->GetAllNickname());
//		client->addMessageToSendbox(RPL_ENDOFNAMES(client->GetUsername(), RealNameChannel));
		NumericReplies::reply::endOfName(*client, RealNameChannel);
client->updateClientStatus(_epollFd);

		return ;
	}
	_channels[RealNameChannel]->AddClient(client->GetNickname(), client->GetFd());
	stringClientMap	ClientChannel = _channels[RealNameChannel]->GetClients();
//	client->addMessageToSendbox(RPL_JOIN(client->GetUsername(), RealNameChannel));
	NumericReplies::Notification::joinNotify(*client, RealNameChannel);
	for (stringClientMap::iterator it = ClientChannel.begin(); it != ClientChannel.end(); it++)
	{
		if (this->_clients.find(it->second) != _clients.end())
		{
//			_clients[it->second]->addMessageToSendbox(RPL_NAMREPLY(_clients[it->second]->GetUsername(), RealNameChannel, _channels.find(RealNameChannel)->second->GetAllNickname()));
			NumericReplies::reply::nameInChannel(*_clients[it->second], RealNameChannel,
					_channels.find(RealNameChannel)->second->GetAllNickname());
//			_clients[it->second]->addMessageToSendbox(RPL_ENDOFNAMES(_clients[it->second]->GetUsername(), RealNameChannel));
			NumericReplies::reply::endOfName(*_clients[it->second], RealNameChannel);
			_clients[it->second]->updateClientStatus(_epollFd);
		}
	}
}
