//
// Created by zorkz on 05/02/24.
//

#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#include <string>

#include "client.hpp"

#define NETWORK_NAME "irc-si-si la famille"

#define RPL_WELCOME		"001"
//#define	RPL_NOTOPIC		"331"
//#define	RPL_TOPIC		"332"
#define	RPL_NAMREPLY	"353"
#define	RPL_ENDOFNAME	"366"
//
//#define ERR_NICKNAMEINUSE	"433"
//#define	ERR_BANNEDFROMCHAN	"474"
//#define ERR_BADCHANNELKEY	"475"

class NumericReplies {
public:
	class reply {
	public:
		static void	welcome(Client &client);
		static void	nameInCHannel(Client &client, const std::string &channName, const std::string &allNick);
		static void	endOfName(Client &client, const std::string &channName);
		static void	joinConfirm(Client &client, const std::string &channName);
	};

	static std::string constructHeader(const std::string &numericID, const std::string &hostName);
};



#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

// NAMES
//# define RPL_NAMREPLY(client, channel, list_of_nicks) (":localhost 353 " + client +"user = #" + channel + " :" + list_of_nicks + "\r\n")
//# define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " #" + channel + " :End of /NAMES list.\r\n")

// JOIN
//# define RPL_JOIN(user, channel) (":" + user + " JOIN #" + channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

// PART
# define RPL_PART(user, channel) (user + " PART #" + channel + " Leaving\r\n")

// TOPIC
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")


// NICK
#endif //NUMERICREPLIES_HPP