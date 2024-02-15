//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::privmsg(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
		throw(NotAuthenticate());
	if (args[1].find("#") != 0)
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
	const std::string UserToSend = args[1];
	try
	{
		for(fdClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->GetNickname() == UserToSend)
			{
				std::stringstream message;
				std::string	content = args[args.size() - 1].substr(args[args.size() - 1].find(":") + 1);
				message << ":" << client->GetNickname() << " PRIVMSG "  << it->second->GetNickname() << " : " << content ;
				it->second->addMessageToSendbox(message.str());
				it->second->updateClientStatus(this->_epollFd);
				std::cout << "message " << content << "send to " << it->second->GetNickname() << std::endl;
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
	const std::string ChannelName(args[1].substr(1));
	try
	{
		if (_channels.empty())
			throw(ChannelNotFound());
		channelMap::iterator	SearchChannel = _channels.find(ChannelName);
		if (SearchChannel == _channels.end())
			throw(ChannelNotFound());
		stringClientMap Clients = SearchChannel->second->GetClients();
		if (Clients.find(client->GetNickname()) == Clients.end())
			throw (NotInTheChannel());
		for (stringClientMap::iterator it = Clients.begin(); it != Clients.end(); it++)
		{
			if (it->second != client->GetFd())
			{
				if (_clients.find(it->second) != _clients.end())
				{
					std::ostringstream message;
					std::string	content = args[args.size() - 1].substr(args[args.size() - 1].find(":") + 1);
					std::cout << "message " << content << "send to " << _clients[it->second]->GetUsername() << std::endl;
					message << user_id(client->GetNickname(), client->GetUsername()) << " PRIVMSG #" << ChannelName << " : " << content << std::endl;
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
