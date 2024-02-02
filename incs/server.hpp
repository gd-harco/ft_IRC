#ifndef SERVER_HPP
#define SERVER_HPP

#define BUFFER_READ_SIZE 5

#include <iostream>
#include <map>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "client.hpp"
#include "channel.hpp"

typedef std::map<int, Client *> fdClientMap;
typedef std::map<std::string, Channel *> channelMap;
typedef std::vector<std::string> vectorCommand;

class Channel;
class Client;

class	Server
{
	public:
	typedef bool (Server::*Handler)(vectorCommand args, Client *client);

		//constructor / destructor
		Server();
		Server(uint64_t port, std::string password);
		Server(const Server &server);
		~Server();

		void	SetMap();

		Server &operator=(const Server &server);

		//getters
		fdClientMap			&GetClients();
		channelMap			& GetChannels();
		uint64_t			GetPort() const;
		int					GetEpollFd() const;
		int					GetScocket() const;
		struct sockaddr_in	GetSockAddr() const;

		//setter
		void	AddClient(int key, Client *clientToAdd);
		void	AddChannel(std::string name, Channel *channel);

		void	RemoveClient(int key);
		void	RemoveChannel(std::string name);

		void	SetPort(uint64_t port);

		//connection
		void	NewConnectionRequest(int fd);
		void	sendMsg(int fd);
		void	HandleEvent(int fd);
		bool	HandleCommand(std::string const &msg, Client *client);

		//commands
		bool	pass(vectorCommand args, Client *client);
		bool	user(vectorCommand args, Client *client);
		bool	nick(vectorCommand args, Client *client);
		bool	kick(vectorCommand args, Client *client);
		bool	invite(vectorCommand args, Client *client);
		bool	topic(vectorCommand args, Client *client);
		bool	mode(vectorCommand args, Client *client);
		bool	privmsg(vectorCommand args, Client *client);
		bool	join(vectorCommand args, Client *client);
		bool	quit(vectorCommand args, Client *client);
		bool	ping(vectorCommand args, Client *client);
		bool	pong(vectorCommand args, Client *client);
		bool	error(vectorCommand args, Client *client);
		bool	part(vectorCommand args, Client *client);


		void	deleteClient(fdClientMap::iterator toDelete) const;
		void	deleteChannel(channelMap::iterator toDelete);
		void	exitservClean();
  
	private:
		std::map<std::string, Handler>	_commands;
		std::string			_password;
		struct epoll_event	_servEpollEvent;
		fdClientMap			_clients;
		channelMap			_channels;
		uint64_t			_port;
		struct sockaddr_in	_sockaddr;
		int					_socket;
		int 				_epollFd;
};

#endif
