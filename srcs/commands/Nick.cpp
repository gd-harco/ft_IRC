//
// Created by zorkz on 04/02/24.
//

#include "server.hpp"

bool	validNickname(const std::string &nick);

//TODO Implement this ?
//The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,
// and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the
// old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
void Server::nick(vectorCommand args, Client *client)
{
	if (args.size() == 1 || args[1].empty()) {
		NumericReplies::Error::noNickGiven(*client);
		client->updateClientStatus(_epollFd);
		throw std::runtime_error("Server::Commands::noNickGiven:  no nick was given");
	}
	if (!validNickname(args[1]))
	{
		NumericReplies::Error::erroneusNickName(*client, args[1]);
		client->updateClientStatus(_epollFd);
		throw ErroneusNickName();
	}
	if (this->_nickUsed.find(args[1]) != this->_nickUsed.end()) {
		NumericReplies::Error::nickInUse(*client, args[1]);
		client->updateClientStatus(this->_epollFd);
		throw NickAlreadyExist();
	}
	if (!client->GetNickname().empty())
		this->_nickUsed.erase(client->GetNickname());
	this->_nickUsed.insert(args[1]);
	if (client->GetNickname().size() != 0)
	{
		NumericReplies::Notification::changeNick(*client, client->GetNickname(), args[1], client->GetUsername());
		client->updateClientStatus(_epollFd);
		for (channelMap::iterator getChannels = _channels.begin(); getChannels != _channels.end(); getChannels++)
		{
			Channel	*channel = getChannels->second;
			stringClientMap clientsChannel = channel->GetClients();
			if (clientsChannel.find(client->GetNickname()) != clientsChannel.end())
			{
				for (stringClientMap::iterator it = clientsChannel.begin(); it != clientsChannel.end(); it++)
				{
					if (client->GetNickname() == it->first)
					{
						if (channel->isOp(client->GetNickname()))
						{
							channel->RemoveOp(client->GetNickname());
							channel->SetOp(args[1]);
						}
						int saveFd = it->second;
						channel->RemoveClient(client->GetNickname());
						channel->AddClient(args[1], saveFd);
					}
					else
					{
						fdClientMap::iterator clientToSendIt = _clients.find(it->second);
						if (clientToSendIt != _clients.end())
						{
							Client *clientToSend = clientToSendIt->second;
							NumericReplies::Notification::changeNick(*clientToSend, client->GetNickname(), args[1], client->GetUsername());
							clientToSend->updateClientStatus(_epollFd);
						}
					}
				}
			}
		}
	}
	client->SetNickname(args[1]);
	std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
}

bool	validNickname(const std::string &nick) {
	if (nick[0] ==  '#' || nick[0] == ':' || nick[0] == '$')
		return false;
	if (nick.find_first_of(" ,*?!@") != std::string::npos)
		return false;
	return true;
}
