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
		ClientMap.erase(client->GetUsername());
		std::cout << client->GetUsername() << " leave channel: " << ChannelName << std::endl;
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

