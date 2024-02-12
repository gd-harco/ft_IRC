#include "server.hpp"
#include "channel.hpp"
#include <algorithm>


void Server::mode(vectorCommand args, Client *client)
{
	if (args.size() < 3)
		throw (Server::NeedMoreParams());
	ModeTopic(args, client);
	ModeOperator(args, client);
}

void Server::ModeOperator(vectorCommand args, Client *client)
{
	if (args[2] == "+o")
	{
		if (args.size() != 5)
			throw(NeedMoreParams());
		std::string const ChannelName(args[1].substr(1));
		channelMap::iterator it = _channels.find(ChannelName);
		if (it ==  _channels.end())
			throw(Server::ChannelNotFound());
		std::vector<std::string> ChannelOperator = it ->second->GetOp();
		std::vector<std::string>::iterator ToFind =  std::find(ChannelOperator.begin(), ChannelOperator.end(),client->GetNickname());
		std::vector<std::string>::iterator NewOpSearch = std::find(ChannelOperator.begin(), ChannelOperator.end(), args[3]);
		if (NewOpSearch != ChannelOperator.end())
		{
			std::cout << "client already operator" << std::endl;
			return ;
		}
		if (ToFind != ChannelOperator.end())
		{
			if (it->second->GetClients().find(args[3]) != it->second->GetClients().end())
			{
				it->second->SetOp(args[3]);
				stringClientMap	ClientChannel = _channels[ChannelName]->GetClients();
				for (stringClientMap::iterator itCM = ClientChannel.begin(); itCM != ClientChannel.end(); itCM++)
				{
					// _clients[itCM->second]->addMessageToSendbox(":localhost MODE #" + ChannelName + " +o" + args[3] + "\r\n");
					NumericReplies::reply::nameInChannel(*_clients[itCM->second], ChannelName, it->second->GetAllNickname());
					NumericReplies::reply::endOfName(*_clients[itCM->second], ChannelName);
					_clients[itCM->second]->updateClientStatus(_epollFd);
				}
			}
			else
				throw (NotInTheChannel());
			return ;
		}
	}
	else if (args[2] == "-o")
	{
		if (args.size() != 5)
			throw(NeedMoreParams());
		std::string const ChannelName(args[1].substr(1));
		channelMap::iterator it = _channels.find(ChannelName);
		if (it ==  _channels.end())
			throw(Server::ChannelNotFound());
		std::vector<std::string> ChannelOperator = it ->second->GetOp();
		std::vector<std::string>::iterator ToFind =  std::find(ChannelOperator.begin(), ChannelOperator.end(),client->GetNickname());
		std::vector<std::string>::iterator NewOpSearch = std::find(ChannelOperator.begin(), ChannelOperator.end(), args[3]);
		if (NewOpSearch == ChannelOperator.end())
		{
			std::cout << "client already not operator" << std::endl;
			return ;
		}
		if (ToFind != ChannelOperator.end())
		{
			if (it->second->GetClients().find(args[3]) != it->second->GetClients().end())
			{
				it->second->RemoveOp(args[3]);
				stringClientMap	ClientChannel = _channels[ChannelName]->GetClients();
				for (stringClientMap::iterator itCM = ClientChannel.begin(); itCM != ClientChannel.end(); itCM++)
				{
					// _clients[itCM->second]->addMessageToSendbox(":localhost MODE #" + ChannelName + " +o" + args[3] + "\r\n");
					NumericReplies::reply::nameInChannel(*_clients[itCM->second], ChannelName, it->second->GetAllNickname());
					NumericReplies::reply::endOfName(*_clients[itCM->second], ChannelName);
					_clients[itCM->second]->updateClientStatus(_epollFd);
				}
			}
			else
				throw (NotInTheChannel());
			return ;
		}
	}
}


void	Server::ModeTopic(vectorCommand args, Client *client)
{
	if (args[2] == "+t")
	{
		if (args.size() < 4)
			throw (Server::NeedMoreParams());
		std::string const ChannelName(args[1].substr(1));
		channelMap::iterator it = _channels.find(ChannelName);
		if (it ==  _channels.end())
			throw(Server::ChannelNotFound());
		std::vector<std::string> ChannelOperator = it ->second->GetOp();
		std::vector<std::string>::iterator ToFind =  std::find(ChannelOperator.begin(), ChannelOperator.end(),client->GetNickname());
		if (ToFind != ChannelOperator.end())
		{
			it->second->SetRTopic(true);
			return ;
		}
		else
			throw (NotInTheChannel());
	}
	else if (args[2] == "-t")
	{
		if (args.size() < 4)
			throw (Server::NeedMoreParams());
		std::string const ChannelName(args[1].substr(1));
		channelMap::iterator it = _channels.find(ChannelName);
		if (it ==  _channels.end())
			throw(Server::ChannelNotFound());
		std::vector<std::string> ChannelOperator = it ->second->GetOp();
		std::vector<std::string>::iterator ToFind =  std::find(ChannelOperator.begin(), ChannelOperator.end(),client->GetNickname());
		if (ToFind != ChannelOperator.end())
		{
			it->second->SetRTopic(false);
			return ;
		}
		else
			throw (NotInTheChannel());
	}
}
