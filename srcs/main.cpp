#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

#include <unistd.h>
#define MAX_EVENT 5

int	main(int argc, char **argv)
{
	if (argc != 2){
		std::cout << "Wrong number of argument" << std::endl;
		return 1;
	}
	uint64_t port = std::strtol(argv[1], NULL, 10);
	Server *serv;
	try {
		serv = new Server(port);
	} catch (std::exception &e){
		std::cout << e.what() << std::endl;
		perror(NULL);
		return (errno);
	}

	std::cout << "Waiting for incoming connection on port " << serv->GetPort() << std::endl;
	struct epoll_event eventsCaught[MAX_EVENT];
	//loop
	while (true) {
		int nfds = epoll_wait(serv->GetEpollFd(), eventsCaught, MAX_EVENT, -1);
		if (nfds == -1){
			perror("epoll wait");
			exit(1);
		}
		for (int i = 0; i < nfds; ++i) {
			int currFd = eventsCaught[i].data.fd;
			if (currFd == serv->GetScocket()) {
				//this mean we got a new incoming connection
				std::cout << "new connection detected" << std::endl;
				serv->NewConnectionRequest(currFd);
			}
			else {
				//si le read fail, alors le client a ete deco
				try {
					serv->HandleMessage(currFd);
				} catch (std::exception &e) {
					std::cout << e.what() << std::endl;
					//parcourir les channels pour voir si il y a le fd et le suppr
					serv->RemoveClient(currFd);
					epoll_ctl(serv->GetEpollFd(), EPOLL_CTL_DEL, currFd, 0);
					close(currFd);
				}
			}
		}
	}
	delete serv;
	return 0;
}
