//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::pass(vectorCommand args, Client *client)
{
	if (client->IsAuthenticate()){
		NumericReplies::Error::alreadyRegistered(*client);

		throw AlreadyRegistred();
	}
	if (args[1] == _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
		client->SetPassword();
		return ;
	}
	else if (args[1] != _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
		std::cout << "socket " << client->GetFd() << " has been closed and associated client deleted" << std::endl;
		throw (BadPassword());
	}
	else if (client->GetPassword() == true)
		throw(AlreadyRegistred());
}
