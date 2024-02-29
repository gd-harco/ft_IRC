//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::user(vectorCommand args, Client *client)
{
	if (args.size() < 5)
	{
		NumericReplies::Error::needMoreParams(*client, "PASS");
		client->updateClientStatus(_epollFd);
		throw(NeedMoreParams());
	}
	if (client->IsAuthenticate()){
		NumericReplies::Error::alreadyRegistered(*client);
		client->updateClientStatus(_epollFd);
		throw AlreadyRegistred();
	}
	if (args[args.size() - 1].find("\r") != args[args.size() - 1].npos)
	{
		std::string	RealName = args[args.size() - 1].substr(1, args[args.size() - 1].find("\r") - 1);
		client->SetRealname(RealName);
		client->SetUsername(args[1]);
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
		return ;
	}
	NumericReplies::Error::needMoreParams(*client, "PASS");
	client->updateClientStatus(_epollFd);
	throw(NeedMoreParams());
}
