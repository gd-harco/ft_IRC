#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <list>
#include <queue>
#include <map>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include "channel.hpp"

#define DELIMITER "\r\n"

class Channel;

class	Client
{
	public:
		//constructor / destructor
		Client();
		explicit Client(int fd);
		Client(int fd, const std::string& username, const std::string& nickname);
		~Client();

		void	updateClientStatus(const int &epollFd);
		void	addClientToEpoll(const int &epollFd);
		void	sendNumericReply(const std::string &message);
		void	addMessageToSendbox(const std::string& message);

		//getters
		int			GetFd() const;
		std::string	GetNickname() const;
		std::string	GetUsername() const;
		std::string	GetRealname() const;
		std::string	GetPingPongToken() const;
		bool		GetPassword() const;
		bool		IsAuthenticate() const;


		//setters
		void	SetPassword();
		void	SetUsername(std::string const &username);
		void	SetNickname(std::string const &nickname);
		void	SetRealname(std::string const &realname);
		void	SetAuthenticate(void);
//		void	SetPingPongToken(std::string const &token);

		void	receiveMsg();
		void	handleString(const std::string &toParse);

	private:
		int			_fd;
		bool		_isInEpoll;
		bool		_password;
		bool		_authenticate;

		std::string				_pingPongToken;
		std::string				_clientBuffer;
		struct epoll_event		_clientEpollevent;
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		std::queue<std::string>	_msgToSend;
};

#endif
