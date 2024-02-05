//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::privmsg(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
		throw(NotAuthenticate());
	if (args[0].find("#") != 0)
	{
		try
		{
			ClientPrivMsg(args, client);
		}
		catch(ClientNotFound &e)
		{
			throw ;
		}
		catch (std::exception &e)
		{
			throw ;
		}
	}
	else
	{
		try
		{
			ChannelPrivMsg(args, client);
		}
		catch (ChannelNotFound &e)
		{
			throw ;
		}
		catch (NotInTheChannel &e)
		{
			throw ;
		}
		catch (std::exception &e)
		{
			throw ;
		}
	}
}

void Server::ClientPrivMsg(vectorCommand args, Client *client)
{
	const std::string UserToSend = args[0];
	try
	{
		for(fdClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->GetUsername() == UserToSend)
			{
				std::ostringstream message;
				std::cout << "message " << args[args.size() - 1] << "send to " << it->second->GetUsername() << std::endl;
				message << client->GetUsername() << " : " << args[args.size() - 1] << std::endl;
				it->second->addMessageToSendbox(message.str());
				it->second->updateClientStatus(this->_epollFd);
				return ;
			}
		}
		throw(ClientNotFound());
	}
	catch (std::exception &e)
	{
		throw ;
	}
}

void Server::ChannelPrivMsg(vectorCommand args, Client *client)
{
	const std::string ChannelName(args[0].substr(1));
	try
	{
		if (_channels.empty())
			throw(ChannelNotFound());
		channelMap::iterator	SearchChannel = _channels.find(ChannelName);
		if (SearchChannel == _channels.end())
			throw(ChannelNotFound());
		stringClientMap Clients = SearchChannel->second->GetClients();
		if (Clients.find(client->GetUsername()) == Clients.end())
			throw (NotInTheChannel());
		for (stringClientMap::iterator it = Clients.begin(); it != Clients.end(); it++)
		{
			if (it->second != client->GetFd())
			{
				if (_clients.find(it->second) != _clients.end())
				{
					std::ostringstream message;
					std::cout << "message " << args[args.size() - 1] << "send to " << _clients[it->second]->GetUsername() << std::endl;
					message << user_id(client->GetNickname(), client->GetUsername()) << " PRIVMSG #" << ChannelName << " : " << args[args.size() - 1] << std::endl;
					_clients[it->second]->addMessageToSendbox(message.str());
					_clients[it->second]->updateClientStatus(this->_epollFd);
				}
			}
		}
	} catch (std::exception &e)
	{
		throw ;
	}
}

