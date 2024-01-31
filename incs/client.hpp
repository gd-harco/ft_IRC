#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <list>
#include <queue>
#include <map>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

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
		bool	pong(Channel &channel)	;
		bool	error(Channel &channel);

		void	updateClientStatus(const int &epollFd);
		void	addClientToEpoll(const int &epollFd);
		void	addMessageToSendbox(std::string message);
		//getters
		int			GetFd() const;
		bool		GetAuthor() const;
		std::string	GetNickname() const;
		std::string	GetUsername() const;

		void	receiveMsg();

	private:
		int			_fd;
		bool		_haveAuthor;
		bool		_isInEpoll;

		struct epoll_event		_clientEpollevent;
		std::string				_ninckname;
		std::string				_username;
		std::queue<std::string>	_msgToSend;
};

#endif
