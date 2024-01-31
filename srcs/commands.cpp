#include "server.hpp"

bool Server::pass(const std::string &args, Client *client)
{

	if (args == _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
		client->SetPassword();
		return false;
	}
	else if (args != _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
		return false;
	}
	else if (client->GetPassword() == true)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have already right password" << std::endl;
		return false;
	}
	return (true);
}

bool Server::nick(const std::string &args, Client *client)
{
	client->SetNickname(args);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
	return (true);
}

bool Server::user(const std::string &args, Client *client)
{
	client->SetUsername(args);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
	return (true);
}

