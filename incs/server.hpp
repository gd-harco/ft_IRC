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
		//constructor / destructor
		Server();
		Server(uint64_t port, std::string password);
		Server(const Server &server);
		~Server();

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
		bool	pass(const std::string &args, Client *client);
		bool	user(const std::string &args, Client *client);
		bool	nick(const std::string &args, Client *client);
		bool	kick(const std::string &args, Client *client);
		bool	invite(const std::string &args, Client *client);
		bool	topic(const std::string &args, Client *client);
		bool	mode(const std::string &args, Client *client);
		bool	privmsg(const std::string &args, Client *client);
		bool	join(const std::string &args, Client *client);
		bool	quit(const std::string &args, Client *client);
		bool	ping(const std::string &args, Client *client);
		bool	pong(const std::string &args, Client *client);
		bool	error(const std::string &args, Client *client);

	private:
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
