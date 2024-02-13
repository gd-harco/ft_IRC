//
// Created by gd-harco on 2/9/24.
//

#include "NumericReplies.hpp"
#include "server.hpp"
void NumericReplies::reply::welcome(Client &client)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_WELCOME, SERVER_NAME)
			<< client.GetUsername()
			<< " :Welcome to the " << NETWORK_NAME
			<< " Network " << client.GetUsername()
			<< "!" + client.GetRealname()
			<< "@localhost" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")
void NumericReplies::reply::noTopic(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_NOTOPIC, SERVER_NAME)
			<< client.GetUsername() << " #"
			<< channName
			<< " :No topic is set" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
void NumericReplies::reply::topic(Client &client, const std::string &channName, const std::string &topic) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_TOPIC, SERVER_NAME)
			<< client.GetUsername() << " #"
			<< channName << " "
			<< topic << DELIMITER;
	client.sendNumericReply(reply.str());
}

//#define RPL_NAMREPLY(client, channel, list_of_nicks) (":localhost 353 " + client +"user = #" + channel + " :" + list_of_nicks + "\r\n")
void	NumericReplies::reply::nameInChannel(Client &client, const std::string &channName, const std::string &allNick) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_NAMREPLY, SERVER_NAME)
			<< client.GetUsername() << "user = #"
			<< channName << " :"
			<< allNick << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")
void	NumericReplies::reply::endOfName(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_ENDOFNAME, SERVER_NAME)
			<< client.GetUsername() << " #"
			<< channName  << " :End of /NAMES list." << DELIMITER;
	client.sendNumericReply(reply.str());
}

//431 * :No nickname given
void	NumericReplies::Error::noNickGiven(Client &client) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NONICKGIVEN, SERVER_NAME)
			<<" * :No nickname given"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

// 433 <nick> :Nickname is already in use"
void	NumericReplies::Error::nickInUse(Client &client, const std::string &nickName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NICKNAMEINUSE, SERVER_NAME)
			<< nickName
			<< " :" << nickName
			<< " is already in use" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//462 ":You may not reregister"
void NumericReplies::Error::alreadyRegistered(Client &client) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_ALREADYREGISTRED, SERVER_NAME)
			<< ":You may not reregister"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}
//# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
void	NumericReplies::Error::bannedFromChan(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_BANNEDFROMCHAN, SERVER_NAME)
			<< client.GetUsername() << " #"
			<< channName << "  :Cannot join channel (+b)" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
void	NumericReplies::Error::badChannelKey(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_BANNEDFROMCHAN, SERVER_NAME)
			<< client.GetUsername() << " #"
			<< channName
			<< " :Cannot join channel (+k)" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//TODO: remove this when implementing notification in respective command file

// # define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
void	NumericReplies::Notification::joinNotify(Client &client, const std::string &channName) {
	client.sendNumericReply(":" + client.GetUsername() + " JOIN #" + channName + DELIMITER);
}

std::string	NumericReplies::constructNumericReplyHeader(const std::string &numericID, const std::string &hostName) {
	std::stringstream   result;

	result << ':' << hostName << ' ' << numericID << ' ';
	return result.str();
}
