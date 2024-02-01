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
typedef std::map<std::string, Channel> channelMap;

class Channel;
class Client;

class	Server
{
	public:
	typedef bool (Server::*Handler)(std::vector<std::string> args, Client *client);

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
		void	AddChannel(std::string name, Channel channel);

		void	RemoveClient(int key);
		void	RemoveChannel(std::string name);

		void	SetPort(uint64_t port);

		//connection
		void	NewConnectionRequest(int fd);
		void	sendMsg(int fd);
		void	HandleEvent(int fd);
		bool	HandleCommand(std::string const &msg, Client *client);

		//commands
		bool	pass(std::vector<std::string> args, Client *client);
		bool	user(std::vector<std::string> args, Client *client);
		bool	nick(std::vector<std::string> args, Client *client);
		bool	kick(std::vector<std::string> args, Client *client);
		bool	invite(std::vector<std::string> args, Client *client);
		bool	topic(std::vector<std::string> args, Client *client);
		bool	mode(std::vector<std::string> args, Client *client);
		bool	privmsg(std::vector<std::string> args, Client *client);
		bool	join(std::vector<std::string> args, Client *client);
		bool	quit(std::vector<std::string> args, Client *client);
		bool	ping(std::vector<std::string> args, Client *client);
		bool	pong(std::vector<std::string> args, Client *client);
		bool	error(std::vector<std::string> args, Client *client);


		void	deleteClient(fdClientMap::iterator toDelete) const;
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
