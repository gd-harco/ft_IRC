//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"
#include "utility.hpp"

//TODO add invitations

void Server::join(vectorCommand args, Client *client)
{
	if (!client->IsAuthenticate())
		throw (NotAuthenticate());

	if (args.size() < 3)
	{
		NumericReplies::Error::needMoreParams(*client, args[0]);
		client->updateClientStatus(_epollFd);
		throw (NeedMoreParams());
	}
	//TODO: Clarifier avec Alex
	if (args[1].find("#") != 0)
	{
		NumericReplies::Error::noSuchChannel(*client, args[1]);
		client->updateClientStatus(_epollFd);
		throw(UnableToCreateChannel());
	}
	std::string const RealNameChannel = processedChannelName(args[1]);
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
	if (_channels[RealNameChannel]->GetUserLimit() >= (int)_channels[RealNameChannel]->GetClients().size() && _channels[RealNameChannel]->GetUserLimit() >= 0)
	{
		NumericReplies::Error::channelIsFull(*client, RealNameChannel);
		client->updateClientStatus(_epollFd);
		return ;
	}
	if (_channels[RealNameChannel]->GetPassword().size() != 0)
	{
		if (args.size() < 4)
		{
			NumericReplies::Error::needMoreParams(*client, args[0]);
			client->updateClientStatus(_epollFd);
			throw(NeedMoreParams());
		}
		if (args[4] != _channels[RealNameChannel]->GetPassword())
		{
			NumericReplies::Error::badChannelKey(*client, RealNameChannel);
			client->updateClientStatus(_epollFd);
		}
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
