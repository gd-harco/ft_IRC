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

std::map<const std::string &, Channel &>	Server::GetChannels() const
{
	return (_channels);
}

std::map<int, Client &> Server::GetClients() const
{
	return (_clients);
}

int Server::GetPort() const
{
	return (_port);
}

