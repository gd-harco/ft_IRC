#include "client.hpp"
#include "utility.hpp"
#include "server.hpp"

extern Server *serv;

Client::Client()
{
}

Client::Client(int fd): _fd(fd), _haveAuthor(false), _isInEpoll(false), _password(false), _authenticate(false)
{
	memset(&this->_clientEpollevent, '\0', sizeof (struct epoll_event));
	this->_clientEpollevent.events = EPOLLIN;
	this->_clientEpollevent.data.fd = fd;
}

Client::Client(int fd, std::string username, std::string nickname): _fd(fd), _nickname(nickname) ,_username(username)
{
	_isInEpoll = false;
	_password = true;
	_authenticate = false;
	memset(&this->_clientEpollevent, '\0', sizeof (struct epoll_event));
	this->_clientEpollevent.events = EPOLLIN;
	this->_clientEpollevent.data.fd = fd;
}

Client::~Client()
{
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

void Client::sendNumericReply(const std::string &message) {
	std::cout << "---------------" << std::endl;
	std::cout << message << std::endl;
	std::cout << "---------------" << std::endl;
	this->_msgToSend.push(message);
}

void Client::addMessageToSendbox(std::string message) {
	if (_password == false)
	{
		std::cout << "client: " << _fd << " dont have password." << std::endl;
		return ;
	}
	if (_nickname.empty())
	{
		std::cout << "client: " << _fd << " dont have nickname." << std::endl;
		return ;
	}
	if (_username.empty())
	{
		std::cout << "client: " << _fd << " dont have username." << std::endl;
		return ;
	}
	this->_msgToSend.push(message);
}

void	Client::handleString(const std::string &toParse) {
	this->_clientBuffer.append(toParse);
	std::queue<std::string> toProcess = split(this->_clientBuffer, DELIMITER);
	while (!toProcess.empty()) {
		//si c'est le dernier message mais qui n'as pas de delimiter, on le met dans le buffer
		if (toProcess.front().find(DELIMITER) == std::string::npos) {
			this->_clientBuffer = toProcess.front();
			toProcess.pop();
			if (!toProcess.empty())
				throw std::runtime_error("Message are left in the queue after a non-terminated message");
			return;
		}
		//TODO this is evil, move all check in the HandelCommand logic please ?
		try {
			if (serv->HandleCommand(toProcess.front(), this) && this->GetPassword() && !this->GetNickname().empty() &&
				!this->GetUsername().empty()) {
				std::cout << "dealt string: " << toProcess.front() << std::endl;
			}
		} catch (Server::BadPassword &e) {
			throw e;
		}
		toProcess.pop();
	}
}


void Client::receiveMsg() {
	while (!_msgToSend.empty()) {
		std::string toSend = _msgToSend.front();
		send(this->_fd, toSend.c_str(), toSend.size(), 0);
		_msgToSend.pop();
	}
}

void Client::SetAuthenticate()
{
	_authenticate = true;
}


void Client::SetPassword()
{
	_password = true;
}
void Client::SetNickname(std::string const &nickname)
{
	_nickname = nickname;
}

void Client::SetUsername(std::string const &username)
{
	_username = username;
}

void Client::SetRealname(std::string const &realname)
{
	_realname = realname;
}


bool Client::GetPassword() const
{
	return (_password);
}

int Client::GetFd() const
{
	return (_fd);
}

bool Client::GetAuthor() const
{
	return (_haveAuthor);
}

std::string Client::GetUsername() const
{
	return (_username);
}

std::string Client::GetNickname() const
{
	return (_nickname);
}

bool Client::IsAuthenticate() const
{
	return (_authenticate);
}

std::string Client::GetRealname() const
{
	return (_realname);
}
