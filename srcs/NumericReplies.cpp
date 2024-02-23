//
// Created by gd-harco on 2/9/24.
//

#include "NumericReplies.hpp"
#include "server.hpp"
void NumericReplies::reply::welcome(Client &client)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_WELCOME, SERVER_NAME)
			<< client.GetNickname()
			<< " :Welcome to the " << NETWORK_NAME
			<< " Network " << client.GetNickname()
			<< "!" + client.GetRealname()
			<< "@localhost" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")
void NumericReplies::reply::noTopic(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_NOTOPIC, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName
			<< " :No topic is set" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
void NumericReplies::reply::topic(Client &client, const std::string &channName, const std::string &topic) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_TOPIC, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName << " "
			<< topic << DELIMITER;
	client.sendNumericReply(reply.str());
}

// RPL_INVINTING 341
void NumericReplies::reply::inviting(Client &client, const std::string &nick, const std::string &channName)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_INVITING, SERVER_NAME)
		<< client.GetNickname() << " "
		<< nick << " #"
		<<channName << DELIMITER;
	client.sendNumericReply(reply.str());

}

//#define RPL_NAMREPLY(client, channel, list_of_nicks) (":localhost 353 " + client +"user = #" + channel + " :" + list_of_nicks + "\r\n")
void	NumericReplies::reply::nameInChannel(Client &client, const std::string &channName, const std::string &allNick) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_NAMREPLY, SERVER_NAME)
			<< client.GetNickname() << " = #"
			<< channName << " :"
			<< allNick << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")
void	NumericReplies::reply::endOfName(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(RPL_ENDOFNAME, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName  << " :End of /NAMES list." << DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::reply::addModeLimit(Client &client, const std::string &channelName, const std::string &number)
{
	std::stringstream reply;
	reply << constructNumericReplyHeader("", SERVER_NAME)
			<< "MODE #" << channelName << " +l " << number << DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::reply::removeModeLimit(Client &client, const std::string &channelName)
{
	std::stringstream reply;
	reply << constructNumericReplyHeader("", SERVER_NAME)
			<< "MODE #" << channelName << " -l" << DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::reply::addModePassword(Client &client, const std::string &channelName, const std::string &password)
{
	std::stringstream reply;
	reply << constructNumericReplyHeader("", SERVER_NAME)
			<< "MODE #" << channelName << " +k " << password << DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::reply::removeModePassword(Client &client, const std::string &channelName)
{
	std::stringstream reply;
	reply << constructNumericReplyHeader("", SERVER_NAME)
			<< "MODE #" << channelName << " -k" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//401 <client nick> <nickname> :No such nick/channel
void NumericReplies::Error::noSuchNick(Client &client, const std::string nickname)
{
	std::stringstream	reply;

	reply << constructNumericReplyHeader(ERR_NOSUCHNICK, SERVER_NAME) << client.GetNickname() << " " << nickname << " :No such nick/channel" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//403 <client> <channel>: No such channel
void NumericReplies::Error::noSuchChannel(Client &client, const std::string channel)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NOSUCHCHANNEL, SERVER_NAME)
		<< client.GetNickname() << " "
		<< "#" << channel
		<< " :No such channel" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//431 * :No nickname given
void	NumericReplies::Error::noNickGiven(Client &client) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NONICKGIVEN, SERVER_NAME)
			<< client.GetNickname() << " "
			<<" * :No nickname given"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}
// 432 <nick> :Erroneus nickname
void	NumericReplies::Error::erroneusNickName(Client &client, const std::string &nickName) {
	std::stringstream reply;
	reply << constructNumericReplyHeader(ERR_ERRONEUSNICKNAME, SERVER_NAME)
			<< client.GetNickname() << " "
			<< nickName
			<< ":Erroneus nickname" << DELIMITER;
	client.sendNumericReply(reply.str());
}

// 433 <nick> :Nickname is already in use
void	NumericReplies::Error::nickInUse(Client &client, const std::string &nickName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NICKNAMEINUSE, SERVER_NAME)
			<< client.GetNickname() << " "
			<< nickName
			<< " :" << nickName
			<< " is already in use" << DELIMITER;
	client.sendNumericReply(reply.str());
}


// 441 <nick> <channel>: They aren't on that channel
void	NumericReplies::Error::userNotInChannel(Client &client, const std::string &nickName, const std::string &channel) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_USERNOTINCHANNEL, SERVER_NAME)
			<< client.GetNickname() << " "
			<< nickName << " #"
			<< channel << " :They aren't on that channel"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

// 442 <channel> : Your not on that channel
void	NumericReplies::Error::notOnChannel(Client &client, const std::string &channel)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NOTONCHANNEL, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channel << " :Your not on that channel"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

// 443 <nick> <channel> : is already on channel
void	NumericReplies::Error::userOnChannel(Client &client, const std::string &nickName, const std::string &channel)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_USERONCHANNEL, SERVER_NAME)
			<< client.GetNickname() << " "
			<< nickName << " #"
			<< channel << " :is already on channel"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

// 461 <command> : Not enough paramters
void	NumericReplies::Error::needMoreParams(Client &client, const std::string &command)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_NOTONCHANNEL, SERVER_NAME)
			<< client.GetNickname() << " "
			<< command << " :Not enough parameters"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}


//462 ":You may not reregister"
void NumericReplies::Error::alreadyRegistered(Client &client) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_ALREADYREGISTRED, SERVER_NAME)
			<< client.GetNickname() << " "
			<< ":You may not reregister"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::Error::channelIsFull(Client &client, const std::string channel)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_CHANNELISFULL, SERVER_NAME) << client.GetNickname() << channel << " you cannot join the channel" << DELIMITER;
	client.sendNumericReply(reply.str());
}


//# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
void	NumericReplies::Error::bannedFromChan(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_BANNEDFROMCHAN, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName << "  :Cannot join channel (+b)" << DELIMITER;
	client.sendNumericReply(reply.str());
}

//# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
void	NumericReplies::Error::badChannelKey(Client &client, const std::string &channName) {
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_BANNEDFROMCHAN, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName
			<< " :Cannot join channel (+k)" << DELIMITER;
	client.sendNumericReply(reply.str());
}

// 482 <client> <channel> :You're not channel operator
void	NumericReplies::Error::chanOpPrivsNeeded(Client &client, const std::string &channName)
{
	std::stringstream reply;

		reply << constructNumericReplyHeader(ERR_NOTONCHANNEL, SERVER_NAME)
			<< client.GetNickname() << " #"
			<< channName << " :You're not channel operator"
			<< DELIMITER;
	client.sendNumericReply(reply.str());
}

void NumericReplies::Error::chanelInviteOnly(Client &client, const std::string &channelName)
{
	std::stringstream reply;

	reply << constructNumericReplyHeader(ERR_INVITEONLYCHAN, SERVER_NAME)
		<< client.GetNickname() << " #"
		<< channelName << " :Channel is only on invite"
		<< DELIMITER;
	client.sendNumericReply(reply.str());
}


//TODO: remove this when implementing notification in respective command file

// # define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
void	NumericReplies::Notification::joinNotify(Client &client, const std::string &channName) {
	client.sendNumericReply(":" + client.GetNickname() + " JOIN #" + channName + DELIMITER);
}

// :fion!caca@localhost KICK #a Carlos :Kicked by the channel's operator
void	NumericReplies::Notification::kickNotify(Client &client, std::string const &name, std::string const &channel ,std::string const &reason)
{
	std::stringstream reply;

	reply << ":" << client.GetNickname() << "!" << client.GetUsername() << "@" << "localhost" << " KICK #" << channel << " " << name << " " << reason << DELIMITER;
	client.sendNumericReply(reply.str());
}

void	NumericReplies::Notification::inviteNotify(Client &client, const std::string &sourceUser, const std::string &channel)
{
	client.sendNumericReply(":" + sourceUser + " INVITE " + client.GetNickname() + " #" + channel + DELIMITER);
}

void	NumericReplies::Notification::pongNotify(Client &client, const std::string &servname, const std::string &token)
{
	client.sendNumericReply("PONG " + servname + " :" + token);
}

void	NumericReplies::Notification::pingNotify(Client &client, const std::string &token)
{
	client.sendNumericReply("PING : " + token);
}

// :fion!caca@localhost NICK caca
void NumericReplies::Notification::changeNick(Client &client, const std::string &oldNick, const std::string &newNick, const std::string &name)
{
	std::stringstream reply;

	reply << ":" << oldNick << "!" << name << "@localhost" << " NICK " << newNick << DELIMITER;
	client.sendNumericReply(reply.str());
}


std::string	NumericReplies::constructNumericReplyHeader(const std::string &numericID, const std::string &hostName) {
	std::stringstream   result;

	result << ':' << hostName << ' ' << numericID << ' ';
	return result.str();
}
