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
	//TODO: Clarifier avec Alex
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
		NumericReplies::Notification::joinNotify(*client, RealNameChannel);
		NumericReplies::reply::nameInChannel(*client, RealNameChannel, NewChannel->GetAllNickname());
		NumericReplies::reply::endOfName(*client, RealNameChannel);
client->updateClientStatus(_epollFd);

		return ;
	}
	_channels[RealNameChannel]->AddClient(client->GetNickname(), client->GetFd());
	stringClientMap	ClientChannel = _channels[RealNameChannel]->GetClients();
	NumericReplies::Notification::joinNotify(*client, RealNameChannel);
	client->updateClientStatus(this->_epollFd);
	for (stringClientMap::iterator it = ClientChannel.begin(); it != ClientChannel.end(); it++)
	{
		if (this->_clients.find(it->second) != _clients.end())
		{
			NumericReplies::reply::nameInChannel(*_clients[it->second], RealNameChannel,
					_channels.find(RealNameChannel)->second->GetAllNickname());
			NumericReplies::reply::endOfName(*_clients[it->second], RealNameChannel);
			_clients[it->second]->updateClientStatus(_epollFd);
		}
	}
}
