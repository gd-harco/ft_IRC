#include "../headers/server.hpp"

Server::Server()
{
}

Server::Server(int port): _port(port)
{
}

Server::~Server()
{
}

std::map<const std::string &, const Channel &>	Server::GetChannels() const
{
	return (_channels);
}

std::map<int, const Client &> Server::GetClients() const
{
	return (_clients);
}

int Server::GetPort() const
{
	return (_port);
}
void Server::AddClient(int key, const Client &client)
{
	_clients.insert(std::make_pair(key, client));
}

void Server::AddChannel(const std::string &name, const Channel &channel)
{
	_channels.insert(std::make_pair(name, channel));
}

void	Server::RemoveChannel(std::string name)
{
	_channels.erase(name);
}

void Server::RemoveClient(int key)
{
	std::map<int, const Client &>	SaveClient = _clients;

	for(std::map<int, const Client &>::iterator it = SaveClient.begin(); it != SaveClient.end(); ++it)
	{
		if (key == it->first)
			_clients.erase(it);
	}
}
