#ifndef SERVER_HPP
#define SERVER_HPP

#define BUFFER_READ_SIZE 512
#define SERVER_NAME "irc.localhost"

#include <iostream>
#include <map>
#include <exception>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "client.hpp"
#include "channel.hpp"
#include "NumericReplies.hpp"

#define MAX_EVENT 5

typedef std::map<int, Client *> fdClientMap;
typedef std::map<std::string, Channel *> channelMap;
typedef std::vector<std::string> vectorCommand;
typedef std::set<std::string> nickSet;

class Channel;
class Client;

class	Server
{
	public:
		typedef void (Server::*Handler)(vectorCommand args, Client *client);

		//constructor / destructor
		Server();
		Server(uint64_t port, std::string password);
		Server(const Server &server);
		~Server();

		void	SetMap();

		void launchServer(void);
		void waitLoop(struct epoll_event eventsCaught[MAX_EVENT]);
		void handleEvent(struct epoll_event event);
		void handleExistingConnection(struct epoll_event event, int currFd);
		void handleEventEpollin(int currFd);

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
		void	CheckConnection(Client *client);

		//commands
		void	pass(vectorCommand args, Client *client);
		void	user(vectorCommand args, Client *client);
		void	nick(vectorCommand args, Client *client);
		void	kick(vectorCommand args, Client *client);
		void	invite(vectorCommand args, Client *client);
		void	topic(vectorCommand args, Client *client);
		void	mode(vectorCommand args, Client *client);
		void	ModeTopic(vectorCommand args, Client *client);
		void	ModeOperator(vectorCommand args, Client *client);
		void	ModeUserLimit(vectorCommand args, Client *client);
		void	ModePassword(vectorCommand args, Client *client);
		void	ModeInvite(vectorCommand args, Client *client);
		void	privmsg(vectorCommand args, Client *client);
		void	ClientPrivMsg(vectorCommand args, Client *client);
		void	ChannelPrivMsg(vectorCommand args, Client *client);
		void	join(vectorCommand args, Client *client);
		void	quit(vectorCommand args, Client *client);
		void	ping(vectorCommand args, Client *client);
		void	pong(vectorCommand args, Client *client);
		void	error(vectorCommand args, Client *client);
		void	part(vectorCommand args, Client *client);

		class NeedMoreParams : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class BadPassword : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class AlreadyRegistred : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class UserAlreadyExist : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class NickAlreadyExist : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class NotAuthenticate : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class ChannelNotFound : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class ClientNotFound : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class UnableToCreateChannel : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class NotAChannel : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};
		class NotInTheChannel : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};

		void	deleteClient(fdClientMap::iterator toDelete) const;
		void	deleteChannel(channelMap::iterator toDelete);
		void	exitservClean();

	private:
		std::map<std::string, Handler>	_commands;
		nickSet							_nickUsed;
		std::string						_password;
		struct epoll_event				_servEpollEvent;
		fdClientMap						_clients;
		channelMap						_channels;
		uint64_t						_port;
		struct sockaddr_in				_sockaddr;
		int								_socket;
		int 							_epollFd;
};

vectorCommand	ParsCommand(std::string const &msg);

#endif
