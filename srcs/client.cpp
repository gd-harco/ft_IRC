#include "client.hpp"

Client::Client()
{
	_password = false;
}

Client::Client(int fd, int epoll_fd): _fd(fd), _epollFd(epoll_fd)
{
	_password = false;
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = _fd;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
}

Client::~Client()
{
}

int Client::GetFd() const
{
	return (_fd);
}

bool Client::GetAuthor() const
{
	return (_haveAuthor);
}

std::string Client::GetNickname() const
{
	return (_ninckname);
}

std::string Client::GetUsername() const
{
	return (_username);
}

bool Client::GetPassword() const
{
	return (_password);
}

void Client::SetNickname(std::string nickname)
{
	_ninckname = nickname;
}

void Client::SetPassword()
{
	_password = true;
}
void Client::SetUsername(std::string username)
{
	_username = username;
}

void Client::SetHaveAuthor(int author)
{
	_haveAuthor = author;
}

