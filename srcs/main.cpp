#include "ServerClass.hpp"

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
	delete serv;
	return 0;
}
