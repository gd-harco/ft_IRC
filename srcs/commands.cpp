#include "server.hpp"



//TODO mettre chaques fonctions dans son fichier pour pouvoir fair des sous fonction et pas galerer apres pour debug

//TODO check le nombre d'args quand le parsing sera fait

bool Server::pass(vectorCommand args, Client *client)
{

	std::cout << client->GetPassword() << std::endl;
	if (args.size() != 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	if (args[0] == _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
		client->SetPassword();
		return true;
	}
	else if (args[0] != _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
		std::cout << "socket " << client->GetFd() << " has been closed and associated client deleted" << std::endl;
		this->RemoveClient(client->GetFd());
		return false;
	}
	else if (client->GetPassword() == true)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have already right password" << std::endl;
		return false;
	}
	return false;
}

bool Server::nick(vectorCommand args, Client *client)
{
	if (args.size() > 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	client->SetNickname(args[0]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
	return (true);
}

bool Server::user(vectorCommand args, Client *client)
{
	if (args.size() < 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	client->SetUsername(args.back());
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
	return (true);
}

bool Server::join(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
	{
		std::cout << client->GetFd() << " is not authenticate" << std::endl;
		return (false);
	}

	// if (args.size() != 1)
	// {
	// 	std::cout << "invalid number of args" << std::endl;
	// 	return (false);
	// }
	if (args[0].find("#") != 0)
	{
		std::cout << "cannot create channel" << std::endl;
	}
	std::string RealNameChannel(args[0].substr(1));
	if (_channels.find(RealNameChannel) == _channels.end())
	{
		std::cout << client->GetUsername() << " create channel " << RealNameChannel << std::endl;
		Channel *NewChannel = new Channel(RealNameChannel);
		NewChannel->AddClient(client->GetUsername(), client->GetFd());
		AddChannel(RealNameChannel, NewChannel);
		return true;
	}
	_channels[RealNameChannel]->AddClient(client->GetUsername(), client->GetFd());
	return true;
}


bool Server::privmsg(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
	{
		std::cout << client->GetFd() << " is not authenticate" << std::endl;
		return (false);
	}
	if (args[0].find("#") != 0)
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
					return (true);
				}
			}
			std::cout << "client: " << UserToSend << "not found 404" << std::endl;
			return (true);
		}
		catch (std::exception &e)
		{
			std::cout << "Error send message to " << UserToSend << ": " << e.what() << std::endl;
			return (false);
		}
	}
	const std::string ChannelName(args[0].substr(1));
	try
	{
		if (_channels.empty())
			return (false);
		channelMap::iterator	SearchChannel = _channels.find(ChannelName);
		if (SearchChannel == _channels.end())
		{
			std::cout << "you cannot send message to " << args[0] << " channel not create" << std::endl;
			return (false);
		}
		stringClientMap Clients = SearchChannel->second->GetClients();
		if (Clients.find(client->GetUsername()) == Clients.end())
		{
			std::cout << "you cannot send message to " << args[0] << " you are not in the channel" << std::endl;
			return (false);
		}
		for (stringClientMap::iterator it = Clients.begin(); it != Clients.end(); it++)
		{
			if (it->second != client->GetFd())
			{
				if (_clients.find(it->second) != _clients.end())
				{
					std::ostringstream message;
					std::cout << "message " << args[args.size() - 1] << "send to " << _clients[it->second]->GetUsername() << std::endl;
					message << client->GetUsername() << " : " << args[args.size() - 1] << std::endl;
					_clients[it->second]->addMessageToSendbox(message.str());
					_clients[it->second]->updateClientStatus(this->_epollFd);
				}
			}
		}
	} catch (std::exception &e)
	{
		std::cout << "Error send message to channel: " << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool Server::part(vectorCommand args, Client *client)
{
	try
	{
		if (args[0][0] != '#')
		{
			std::cout << args[0] << " is not a channel" << std::endl;
		}
		std::string const ChannelName(args[0].substr(1));
		const channelMap::iterator ChannelIt =  _channels.find(ChannelName);
		if (ChannelIt == _channels.end())
		{
			std::cout << "channel: " << ChannelName << " not found" << std::endl;
			return (false);
		}
		stringClientMap ClientMap = ChannelIt->second->GetClients();
		const stringClientMap::iterator ClientMapIt = ClientMap.find(client->GetUsername());
		if (ClientMapIt == ClientMap.end())
		{
			std::cout << client->GetUsername() << " are not in the channel: " << ChannelName << std::endl;
			return (false);
		}
		ClientMap.erase(client->GetUsername());
		std::cout << client->GetUsername() << " leave channel: " << ChannelName << std::endl;
		if (ClientMap.empty())
		{
			delete _channels[ChannelName];
			_channels.erase(ChannelName);
			std::cout << "delete: #" << ChannelName << std::endl;
		}
		return (true);
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (true);
}

