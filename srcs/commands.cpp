#include "server.hpp"

bool Server::pass(std::vector<std::string> args, Client *client)
{

	if (args.size() != 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	if (args[0] == _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
		client->SetPassword();
		return true;
	}
	else if (args[0] != _password && client->GetPassword() == false)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
		std::cout << "socket " << client->GetFd() << " has been closed and associated client deleted" << std::endl;
		this->RemoveClient(client->GetFd());
		return false;
	}
	else if (client->GetPassword() == true)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have already right password" << std::endl;
		return false;
	}
	return false;
}

bool Server::nick(std::vector<std::string> args, Client *client)
{
	if (args.size() > 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	client->SetNickname(args[0]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
	return (true);
}

bool Server::user(std::vector<std::string> args, Client *client)
{
	if (args.size() < 2)
	{
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "invalid arg number" << std::endl;
		return false;
	}
	client->SetUsername(args.back());
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
	return (true);
}

