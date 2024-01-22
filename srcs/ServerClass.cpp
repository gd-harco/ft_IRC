//
// Created by Guillaume on 21/01/2024.
//

#include "ServerClass.hpp"


ServerClass::ServerClass(uint64_t port) {
	_port = port;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw std::runtime_error("Failed to create socket");
	}

	_epollFd = epoll_create(5);
	if (_epollFd == -1) {
		throw std::runtime_error("Failed to create epoll file descriptor");
	}

	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_port = htons(this->_port);
	_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(this->_socket, (struct sockaddr *)(&this->_sockaddr), sizeof(_sockaddr)) == -1) {
		throw std::runtime_error("Failed to bind socket");
	}

	if (listen(this->_socket, 5) == -1) {
		throw std::runtime_error("Failed to listen on socket");
	}
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

const sockaddr_in &ServerClass::getSockaddr() const {
	return _sockaddr;
}

void ServerClass::setSockaddr(const sockaddr_in &sockaddr) {
	_sockaddr = sockaddr;
}
