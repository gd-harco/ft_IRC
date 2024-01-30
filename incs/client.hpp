#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <list>
#include <map>
#include "channel.hpp"
#include <sys/epoll.h>


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
		Client(int fd, int epoll_fd);
		~Client();

		//getters
		int			GetFd() const;
		bool		GetAuthor() const;
		bool		GetPassword() const;
		std::string	GetNickname() const;
		std::string	GetUsername() const;

		//setters
		void	SetPassword();
		void	SetHaveAuthor(int author);
		void	SetNickname(std::string nickname);
		void	SetUsername(std::string username);
		void	AddMsgToSend(std::string msg);

	private:
		int						_fd;
		int						_epollFd;
		bool					_password;
		int						_haveAuthor;
		std::string				_ninckname;
		std::string				_username;
		std::list<std::string>	_msgToSend;
		struct epoll_event		_event;
};

#endif
