//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::nick(vectorCommand args, Client *client)
{
	if (args.size() == 1 || args[1].empty())
		NumericReplies::Error::noNickGiven(*client);
	if (this->_nickUsed.find(args[1]) != this->_nickUsed.end()) {
		NumericReplies::Error::nickInUse(*client, args[1]);
		client->updateClientStatus(this->_epollFd);
		throw NickAlreadyExist();
	}
	if (!client->GetNickname().empty())
		this->_nickUsed.erase(client->GetNickname());
	this->_nickUsed.insert(args[1]);
	client->SetNickname(args[1]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
}
