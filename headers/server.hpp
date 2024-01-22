#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <string>
#include "client.hpp"
#include "channel.hpp"

class Channel;
class Client;

class	Server
{
	private:
		std::map<int, Client>	_clients;
		std::map<std::string, Channel>	_channels;
		int	_port;

	public:
		//constructor / destructor
		Server();
		Server(int port);
		~Server();

		//getters
		std::map<int, Client >	&GetClients();
		std::map<std::string , Channel >	&GetChannels();
		int			GetPort() const;

		//setter
		void	AddClient(int key, Client clent);
		void	AddChannel(std::string name, Channel channel);

		void	RemoveClient(int key);
		void	RemoveChannel(std::string name);

		void	SetPort(int port);
};

#endif