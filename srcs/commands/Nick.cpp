//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::nick(vectorCommand args, Client *client)
{
	if (args.size() > 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		throw (NeedMoreParams());
	}
	client->SetNickname(args[0]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
}
