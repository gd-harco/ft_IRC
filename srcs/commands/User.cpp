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
	for (fdClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
	{}
	client->SetUsername(args.back());
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
}