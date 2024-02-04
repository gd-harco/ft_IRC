//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::pass(vectorCommand args, Client *client)
{
	if (args.size() != 2)
		throw(NeedMoreParams());
	if (args[0] == _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
		client->SetPassword();
		return ;
	}
	else if (args[0] != _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
		std::cout << "socket " << client->GetFd() << " has been closed and associated client deleted" << std::endl;
		this->RemoveClient(client->GetFd());
		throw (BadPassword());
	}
	else if (client->GetPassword() == true)
		throw(AlreadyRegistred());
}