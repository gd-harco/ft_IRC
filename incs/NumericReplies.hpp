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
#define	RPL_NAMREPLY	"353"
#define	RPL_ENDOFNAME	"366"
//
#define ERR_NICKNAMEINUSE	"433"
#define	ERR_BANNEDFROMCHAN	"474"
#define ERR_BADCHANNELKEY	"475"

class NumericReplies {
public:
	//TODO: remove this when implementing notification in respective command file
	class Notification {
	public:
		static void	joinNotify(Client &client, const std::string &channName);
	};

	class reply {
	public:
		static void	welcome(Client &client);
		static void	noTopic(Client &client, const std::string &channName);
		static void topic(Client &client, const std::string &channName, const std::string &topic);
		static void	nameInChannel(Client &client, const std::string &channName, const std::string &allNick);
		static void	endOfName(Client &client, const std::string &channName);
	};

	class Error {
	public:
		static void nickInUse(Client &client, const std::string &nickName);
		static void	bannedFromChan(Client &client, const std::string &channName);
		static void	badChannelKey(Client &client, const std::string &channName);
	};

	static std::string constructNumericReplyHeader(const std::string &numericID, const std::string &hostName);
};



#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

//TODO: not a NumericReply but a notification, moove it in the corresponding command file
//# define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
# define RPL_PART(user, channel) (user + " PART #" + channel + " Leaving\r\n")


#endif //NUMERICREPLIES_HPP
