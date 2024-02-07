//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::user(vectorCommand args, Client *client)
{
	if (args.size() < 2)
	{
		throw (NeedMoreParams());
	}
	// for (fdClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
	// {}
	client->SetRealname(args[args.size() - 1]);
	client->SetUsername(args[0]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
}