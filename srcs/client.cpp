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
