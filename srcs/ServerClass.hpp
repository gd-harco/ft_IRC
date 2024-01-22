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

#include <cstdio>
#include <cstdlib>

class ServerClass {
public:
	ServerClass(uint64_t port);
	ServerClass(const ServerClass &obj);
	~ServerClass();
	ServerClass &operator=(const ServerClass &rhs);
private:
	uint64_t			_port;
	int					_socket;
	int 				_epollFd;
	struct sockaddr_in	_sockaddr;
public:
	const sockaddr_in &getSockaddr() const;

	void setSockaddr(const sockaddr_in &sockaddr);
};


#endif //SERVERCLASS_HPP
