#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include "client.hpp"
#include "channel.hpp"

class Channel;
class Client;

class	Server
{
	public:
		//constructor / destructor
		Server();
		Server(int port);
		~Server();

		//getters
		std::map<int, Client &>	GetClients() const;
		std::map<const std::string &, Channel &>						GetChannels() const;
		int			GetPort() const;

	private:
		std::map<int, Client &>	_clients;
		std::map<const std::string &, Channel &>	_channels;
		int	_port;

};

#endif