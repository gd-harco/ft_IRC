#include "ServerClass.hpp"

#include <unistd.h>
#define MAX_EVENT 5

int	main(int argc, char **argv) {
	if (argc != 2){
		std::cout << "Wrong number of argument" << std::endl;
		return 1;
	}
	uint64_t port = std::strtol(argv[1], NULL, 10);
	ServerClass *serv;
	try {
		serv = new ServerClass(port);
	} catch (std::exception &e){
		std::cout << e.what() << std::endl;
		perror(NULL);
		return (errno);
	}

	std::cout << "Waiting for incoming connection on port " << serv->_port << std::endl;
	struct epoll_event eventsCaught[MAX_EVENT];
	//loop
	while (1) {
		int nfds = epoll_wait(serv->_epollFd, eventsCaught, MAX_EVENT, -1);
		if (nfds == -1){
			perror("epoll wait");
			exit(1);
		}
		for (int i = 0; i < nfds; ++i) {
			int currFd = eventsCaught[i].data.fd;
			if (currFd == serv->_socket) {
				//this mean we got a new incoming connection
				std::cout << "new connection detected" << std::endl;
				serv->newConnectionRequest(currFd);
			}
			else {
				//si le read fail, alors le client a ete deco
				try {
					serv->handleMessage(currFd);
				} catch (std::exception &e) {
					;
				}
			}
		}
	}
	delete serv;
	return 0;
}
