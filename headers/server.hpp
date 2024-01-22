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
	public:
		//constructor / destructor
		Server();
		Server(int port);
		~Server();

		//getters
		std::map<int, const Client &>	GetClients() const;
		std::map<const std::string &, const Channel &>	GetChannels() const;
		int			GetPort() const;

		//setter
		void	AddClient(int key, const Client &clent);
		void	AddChannel(const std::string &name, const Channel &channel);

		void	RemoveClient(int key);
		void	RemoveChannel(std::string name);
	private:
		std::map<int, const Client &>	_clients;
		std::map<const std::string &, const Channel &>	_channels;
		int	_port;

};

#endif