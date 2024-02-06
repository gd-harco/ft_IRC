//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::part(vectorCommand args, Client *client)
{
	try
	{
		if (args[0][0] != '#')
			throw(NotAChannel());
		std::string const ChannelName(args[0].substr(1));
		const channelMap::iterator ChannelIt =  _channels.find(ChannelName);
		if (ChannelIt == _channels.end())
			throw (ChannelNotFound());
		stringClientMap ClientMap = ChannelIt->second->GetClients();
		const stringClientMap::iterator ClientMapIt = ClientMap.find(client->GetUsername());
		if (ClientMapIt == ClientMap.end())
			throw(NotInTheChannel());
		//:aaaa!zizi@localhost PART #a

		stringClientMap	ClientChannel = _channels[ChannelName]->GetClients();
		for (stringClientMap::iterator it = ClientChannel.begin(); it != ClientChannel.end(); it++)
		{
			if (_clients.find(it->second) != _clients.end())
			{
				_clients[it->second]->addMessageToSendbox(RPL_PART(user_id(client->GetNickname(), client->GetUsername()), ChannelName));
				_clients[it->second]->updateClientStatus(_epollFd);
			}
		}
		this->_channels.find(ChannelName)->second->RemoveClient(client->GetUsername());
		ClientMap.erase(client->GetUsername());
		std::cout << RPL_PART(user_id(client->GetNickname(), client->GetUsername()), ChannelName) << std::endl;
		if (ClientMap.empty())
		{
			delete _channels[ChannelName];
			_channels.erase(ChannelName);
			std::cout << "delete: #" << ChannelName << std::endl;
		}
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}

