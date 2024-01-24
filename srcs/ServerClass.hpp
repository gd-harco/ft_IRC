//
// Created by Guillaume on 21/01/2024.
//

#ifndef SERVERCLASS_HPP
#define SERVERCLASS_HPP


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>

#include <vector>
#include <cstdio>
#include <cstdlib>

class ServerClass {
public:
	ServerClass(uint64_t port);
	ServerClass(const ServerClass &obj);
	~ServerClass();
	ServerClass &operator=(const ServerClass &rhs);
	uint64_t			_port;
	struct sockaddr_in	_sockaddr;
	int 				_epollFd;
	int					_socket;
	std::vector<int>	_clientsFd;

	void	newConnectionRequest(int fd);
	void	handleMessage(int fd);
};


#endif //SERVERCLASS_HPP
