#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

Server *serv;


void	handleSigInt(int sig);

int	main(int argc, char **argv)
{
	if (argc != 3){
		std::cout << "Wrong number of argument, usage: ./irc <port> <password>" << std::endl;
		return 1;
	}
	uint64_t port = std::strtol(argv[1], NULL, 10);
	try {
		serv = new Server(port, std::string(argv[2]));
	} catch (std::exception &e){
		std::cout << e.what() << std::endl;
		perror(NULL);
		return (errno);
	}

	serv->launchServer();
	return (0);
}

void	handleSigInt(int sig)
{
	serv->exitservClean();
	delete serv;
	std::cout << "exiting du to catching SIGNAL " << sig << std::endl;
	exit(0);
}
