//
// Created by Guillaume on 21/01/2024.
//

#include "ServerClass.hpp"
#include <cstring>

	ServerClass::ServerClass(uint64_t port) {
		_port = port;
		_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == -1) {
			throw std::runtime_error("Failed to create socket");
		}
		std::cout << "Created a socket with fd: " << this->_socket << std::endl;
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_port =htons(this->_port);
		_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(this->_socket, (struct sockaddr *)(&this->_sockaddr), sizeof(_sockaddr)) == -1) {
			throw std::runtime_error("Failed to bind socket");
		}

		if (listen(this->_socket, 5) == -1) {
			throw std::runtime_error("Failed to listen on socket");
		}

		_epollFd = epoll_create(5);
		if (_epollFd == -1) {
			throw std::runtime_error("Failed to create epoll file descriptor");
		}

		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = this->_socket;
		epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socket, &event);
	}


ServerClass::ServerClass(const ServerClass &obj) {
	if (this != &obj)
		*this = obj;
}

ServerClass::~ServerClass() {
}

ServerClass &ServerClass::operator=(const ServerClass &rhs) {
	if (this != &rhs) {
		//copy code goes here
	}
	return *this;
}

void	ServerClass::newConnectionRequest(int fd) {
		struct sockaddr_in	new_addr;
		int					addrLen = sizeof(new_addr);

		int	client = accept(fd, (struct sockaddr*)&new_addr,
			(socklen_t*)&addrLen);
		this->_clientsFd.push_back(client);
		struct epoll_event event;
		event.events = EPOLLIN | EPOLLET;
		event.data.fd = client;
		epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, client, &event);
	}

void	ServerClass::handleMessage(int fd) {
	// todo virer ce while 1, voire si epoll
	char buf[10 + 1];
	std::string	msg;
	while(int ret_data = recv(fd, buf, 10, 0) != 0) {

		std::memset(buf, 0, 11);
		msg = msg + std::string(buf);
		std::cout << "curr string : " << msg << std::endl;
	}
	std::cout << msg << std::endl;
}
