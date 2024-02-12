//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"


//TODO Implement this ?
//The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,
// and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the
// old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
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
