//
// Created by zorkz on 05/02/24.
//

#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

#include "client.hpp"

#define NETWORK_NAME "irc-si-si la famille"

#define RPL_WELCOME		"001"
#define	RPL_NOTOPIC		"331"
#define	RPL_TOPIC		"332"
#define RPL_INVITING	"341"
#define	RPL_NAMREPLY	"353"
#define	RPL_ENDOFNAME	"366"
//
#define	ERR_NOSUCHNICK			"401"
#define ERR_NOSUCHCHANNEL		"403"
#define ERR_NONICKGIVEN			"431"
#define ERR_ERRONEUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_USERNOTINCHANNEL	"441"
#define ERR_NOTONCHANNEL		"442"
#define ERR_USERONCHANNEL		"443"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_ALREADYREGISTRED	"462"
#define ERR_CHANNELISFULL		"471"
#define ERR_INVITEONLYCHAN		"473"
#define	ERR_BANNEDFROMCHAN		"474"
#define ERR_BADCHANNELKEY		"475"
#define ERR_CHANOPRIVSNEEDED	"482"


class NumericReplies {
public:
	//TODO: remove this when implementing notification in respective command file
	class Notification {
	public:
		static void	joinNotify(Client &client, const std::string &channName);
		static void kickNotify(Client &client, const std::string &sourceUser, const std::string &channel, const std::string reason);
		static void inviteNotify(Client &client, const std::string &sourceUser, const std::string &channel);
	};

	class reply {
	public:
		static void	welcome(Client &client);
		static void	noTopic(Client &client, const std::string &channName);
		static void topic(Client &client, const std::string &channName, const std::string &topic);
		static void inviting(Client &client, const std::string &nick, const std::string &channName);
		static void	nameInChannel(Client &client, const std::string &channName, const std::string &allNick);
		static void	endOfName(Client &client, const std::string &channName);
		static void	removeModeLimit(Client &client, const std::string &channelName);
		static void	addModeLimit(Client &client, const std::string &channelName, const std::string &number);
		static void removeModePassword(Client &client, const std::string &channelName);
		static void	addModePassword(Client &client, const std::string &channelName, const std::string &password);
	};

	class Error {
	public:
		static void	noSuchNick(Client &client, const std::string nickname);
		static void noSuchChannel(Client &client, const std::string channel);
		static void	noNickGiven(Client &client);
		static void	erroneusNickName(Client &client, const std::string &nickName);
		static void nickInUse(Client &client, const std::string &nickName);
		static void userNotInChannel(Client &client, const std::string &nickName, const std::string &channel);
		static void notOnChannel(Client &client, const std::string &channel);
		static void userOnChannel(Client &client, const std::string &nickName, const std::string &channel);
		static void needMoreParams(Client &client, const std::string &command);
		static void	alreadyRegistered(Client &client);
		static void	channelIsFull(Client &client, const std::string channel);
		static void	bannedFromChan(Client &client, const std::string &channName);
		static void	badChannelKey(Client &client, const std::string &channName);
		static void chanOpPrivsNeeded(Client &client, const std::string &channName);
		static void	chanelInviteOnly(Client &client, const std::string &channelName);

	};

	static std::string constructNumericReplyHeader(const std::string &numericID, const std::string &hostName);
};



#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

//TODO: not a NumericReply but a notification, moove it in the corresponding command file
//# define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
# define RPL_PART(user, channel) (user + " PART #" + channel + " Leaving\r\n")


#endif //NUMERICREPLIES_HPP
