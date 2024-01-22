#include "../headers/client.hpp"

Client::Client()
{
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
