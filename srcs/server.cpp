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

std::map<std::string , Channel >	&Server::GetChannels()
{
	return (_channels);
}

std::map<int, Client > &Server::GetClients()
{
	return (_clients);
}

int Server::GetPort() const
{
	return (_port);
}

void Server::SetPort(int port)
{
	_port = port;
}


void Server::AddClient(int key, Client client)
{
	this->_clients[key] = client;
}

void Server::AddChannel(std::string name, Channel channel)
{
	_channels[name] = channel;
}

void	Server::RemoveChannel(std::string name)
{
	_channels.erase(name);
}

void Server::RemoveClient(int key)
{
	std::map<int, Client >	SaveClient = _clients;

	for(std::map<int, Client >::iterator it = SaveClient.begin(); it != SaveClient.end(); ++it)
	{
		if (key == it->first)
			_clients.erase(key);
	}
}
