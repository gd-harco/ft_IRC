//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::user(vectorCommand args, Client *client)
{
	// for (fdClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
	// {}
	std::string	RealName = args[args.size() - 1].substr(1, args[args.size() - 1].find("\r") - 1);
	client->SetRealname(RealName);
	client->SetUsername(args[1]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
}