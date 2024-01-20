//https://broux.developpez.com/articles/c/sockets/

//
// Created by gd-harco on 1/20/24.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv){
	(void)argc;
	//create a socket
	uint16_t port = std::strtol(argv[1], NULL, 10);
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET; //address IPV4
	addr.sin_port = htons(port); // Port number
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // Any available IP address

	//lie le socket a un port
	if (bind(socket_fd, (struct sockaddr *)(&addr), sizeof(addr) )!= 0){
		perror("Error binding socket");
		exit(errno);
	}
	//met le socket en mode ecoute (dis au programme "ce socket est dipsoni
	if (listen(socket_fd, 5) == -1){
		perror("Error puting socket in listen mode");
		exit(errno);
	};

	int epollFd = epoll_create(1);
	if (epollFd == -1){
		perror("Error epoll_create()");
		exit(errno);
	};
}
//https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642
