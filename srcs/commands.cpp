#include "server.hpp"

bool Server::pass(std::vector<std::string> args, Client *client)
{

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

bool Server::nick(std::vector<std::string> args, Client *client)
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

bool Server::user(std::vector<std::string> args, Client *client)
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

bool Server::join(std::vector<std::string> args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
	{
		std::cout << client->GetFd() << " is not authenticate" << std::endl;
		return (false);
	}
	std::cout << args.size() << std::endl;
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


bool Server::privmsg(std::vector<std::string> args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
	{
		std::cout << client->GetFd() << " is not authenticate" << std::endl;
		return (false);
	}
	if (args[0].find("#") != 0)
	{
		std::cout << "MESSAGE WITH NO CHANNEL NO CREATE FOR NOW" << std::endl;
	}
	std::string ChannelName(args[0].substr(1));
	try
	{
		stringClientMap Clients = this->_channels[ChannelName]->GetClients();
		for (stringClientMap::iterator it = Clients.begin(); it != Clients.end(); it++)
		{
			if (it->second != client->GetFd())
			{
				if (_clients.find(it->second) != _clients.end())
				{
					std::cout << "message " << args[args.size() - 1] << "send to " <<
					_clients[it->second]->GetUsername() << std::endl;
					_clients[it->second]->addMessageToSendbox(args[args.size() - 1]);
					_clients[it->second]->updateClientStatus(this->_epollFd);
				}
			}
		}
	} catch (std::exception &e)
	{
		std::cout << "Error send message to channel " << e.what() << std::endl;
	}
	return (true);
}

