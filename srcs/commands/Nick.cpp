//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::nick(vectorCommand args, Client *client)
{
	client->SetNickname(args[1]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
}
