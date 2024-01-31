#include "client.hpp"

Client::Client()
{
}

Client::Client(int fd): _fd(fd), _isInEpoll(false), _ninckname("johny")
{
	bzero(&this->_clientEpollevent, sizeof (struct epoll_event));
	this->_clientEpollevent.events = EPOLLIN;
	this->_clientEpollevent.data.fd = fd;
}

Client::Client(std::string username, std::string nickname): _ninckname(nickname) ,_username(username)
{
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

void	Client::addClientToEpoll(const int &epollFd) {
	epoll_ctl(epollFd, EPOLL_CTL_ADD, this->_fd, &this->_clientEpollevent);
}

void	Client::updateClientStatus(const int &epollFd) {
	if (!_isInEpoll)
	{
		epoll_ctl(epollFd, EPOLL_CTL_ADD, this->_fd, &this->_clientEpollevent);
		_isInEpoll = true;
		return;
	}
	if (this->_msgToSend.empty())
		this->_clientEpollevent.events = EPOLLIN;
	else
		this->_clientEpollevent.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(epollFd, EPOLL_CTL_MOD, this->_fd, &this->_clientEpollevent);
}

void Client::addMessageToSendbox(std::string message) {
	this->_msgToSend.push(message);
}

void Client::receiveMsg() {
	std::string toSend = _msgToSend.front();
	send(this->_fd, toSend.c_str(), toSend.size(), 0);
	_msgToSend.pop();
}
