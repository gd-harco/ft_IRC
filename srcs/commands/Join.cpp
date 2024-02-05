//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

void Server::join(vectorCommand args, Client *client)
{
	if (client->GetUsername().empty() || client->GetNickname().empty() || client->GetPassword() == false)
		throw (NotAuthenticate());

	// if (args.size() != 1)
	// {
	// 	std::cout << "invalid number of args" << std::endl;
	// 	return (false);
	// }
	if (args[0].find("#") != 0)
		throw(UnableToCreateChannel());
	std::string RealNameChannel(args[0].substr(1));
	if (_channels.find(RealNameChannel) == _channels.end())
	{
		std::cout << client->GetUsername() << " create channel " << RealNameChannel << std::endl;
		Channel *NewChannel = new Channel(RealNameChannel);
		NewChannel->AddClient(client->GetUsername(), client->GetFd());
		AddChannel(RealNameChannel, NewChannel);
		client->addMessageToSendbox(":irc.localhost 353 " + client->GetUsername() +"user = #" + RealNameChannel + " :" + client->GetNickname() + "\r\n");
		client->addMessageToSendbox(":irc.localhost 366 " + client->GetUsername() +" #" + RealNameChannel + " :End of /NAMES list.\r\n");
		client->addMessageToSendbox(":" + client->GetUsername() + " JOIN #" + RealNameChannel + "\r\n");
		client->updateClientStatus(_epollFd);

		return ;
	}
	_channels[RealNameChannel]->AddClient(client->GetUsername(), client->GetFd());
}

