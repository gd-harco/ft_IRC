//
// Created by gd-harco on 2/9/24.
//

#include "NumericReplies.hpp"
#include "server.hpp"
void NumericReplies::reply::welcome(Client &client)
{
	client.addMessageToSendbox(constructHeader(RPL_WELCOME, SERVER_NAME) + client.GetUsername() \
	+ " :Welcome to the " + NETWORK_NAME + " Network " + client.GetUsername() + "!" + client.GetRealname() \
	+ "@localhost\r\n");
}

std::string NumericReplies::constructHeader(const std::string &numericID, const std::string &hostName) {
	std::stringstream   result;

	result << ':' << hostName << ' ' << numericID << ' ';
	return result.str();
}
