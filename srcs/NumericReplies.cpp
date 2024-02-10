//
// Created by gd-harco on 2/9/24.
//

#include "NumericReplies.hpp"
#include "server.hpp"
void NumericReplies::reply::welcome(Client &client)
{
	client.addMessageToSendbox(constructHeader(RPL_WELCOME, SERVER_NAME) + client.GetUsername() \
	+ " :Welcome to the " + NETWORK_NAME + " Network " + client.GetUsername() + "!" + client.GetRealname() \
	+ "@localhost" + DELIMITER);
}

//#define RPL_NAMREPLY(client, channel, list_of_nicks) (":localhost 353 " + client +"user = #" + channel + " :" + list_of_nicks + "\r\n")
void	NumericReplies::reply::nameInCHannel(Client &client, const std::string &channName, const std::string &allNick) {
	client.addMessageToSendbox(constructHeader(RPL_NAMREPLY, SERVER_NAME) + client.GetUsername() \
 + "user = #" + channName + " :" + allNick + DELIMITER);
}

//# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")
void	NumericReplies::reply::endOfName(Client &client, const std::string &channName) {
	client.addMessageToSendbox(constructHeader(RPL_ENDOFNAME, SERVER_NAME) + client.GetUsername() \
	+ " #" + channName + " :End of /NAMES list." + DELIMITER);
}


//# define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
void NumericReplies::Notification::joinNotify(Client &client, const std::string &channName) {
	client.addMessageToSendbox(":" + client.GetUsername() + " JOIN #" + channName + DELIMITER);
}



std::string NumericReplies::constructHeader(const std::string &numericID, const std::string &hostName) {
	std::stringstream   result;

	result << ':' << hostName << ' ' << numericID << ' ';
	return result.str();
}
