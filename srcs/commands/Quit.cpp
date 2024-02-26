//
// Created by gd-harco on 26/02/24.
//

#include "server.hpp"

void Server::quit(vectorCommand args, Client *client)
{
	//TODO Mettre la bonne NR
	client->_toDestroy = true;
	NumericReplies::Notification::error(*client, "Quit :" + args[1]);
	client->updateClientStatus(this->_epollFd);
}
