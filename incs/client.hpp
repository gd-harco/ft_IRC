#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <list>
#include <map>
#include "channel.hpp"


// Numeric Replies:

//     ERR_NOSUCHNICK (401)
//     ERR_NOSUCHSERVER (402)
//     ERR_CANNOTSENDTOCHAN (404)
//     ERR_TOOMANYTARGETS (407)
//     ERR_NORECIPIENT (411)
//     ERR_NOTEXTTOSEND (412)
//     ERR_NOTOPLEVEL (413)
//     ERR_WILDTOPLEVEL (414)
//     RPL_AWAY (301)
//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_NOTONCHANNEL (442)
//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_NOTONCHANNEL (442)
//     ERR_CHANOPRIVSNEEDED (482)
//     RPL_NOTOPIC (331)
//     RPL_TOPIC (332)
//     RPL_TOPICWHOTIME (333)


class Channel;

class	Client
{
	public:
		//constructor / destructor
		Client();
		Client(int fd);
		Client(std::string username, std::string nickname);
		~Client();

		//commands
		bool	kick(const std::string &KickUser);
		bool	invite(const std::string &InvitUser);
		bool	topic(const std::string &Channel);
		bool	mode(const unsigned char type, const std::string &Channel);
		bool	privmsg(const std::string &msg, Channel &channel);
		bool	join(Channel &channel);
		bool	quit(Channel &channel);
		bool	ping(Channel &channel);
		bool	pong(Channel &channel);
		bool	error(Channel &channel);

		//getters
		int			GetFd() const;
		bool		GetAuthor() const;
		std::string	GetNickname() const;
		std::string	GetUsername() const;

	private:
		int			_fd;
		bool		_haveAuthor;
		std::string	_ninckname;
		std::string	_username;
		std::list<std::string>	_msgToSend;
};

#endif
