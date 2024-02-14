#include "server.hpp"
#include <unistd.h>

Server::Server()
{
}

Server::Server(const Server &server)
{
	*this = server;
}

Server::Server(uint64_t port, std::string password)
{
	int	option = 1;
	_port = port;
	_password = password;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw std::runtime_error("Failed to create socket");
	}
	setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	std::cout << "Created a socket with fd: " << this->_socket << std::endl;
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_port =htons(this->_port);
	_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(this->_socket, (struct sockaddr *)(&this->_sockaddr), sizeof(_sockaddr)) == -1) {
		throw std::runtime_error("Failed to bind socket");
	}

	if (listen(this->_socket, 5) == -1) {
		throw std::runtime_error("Failed to listen on socket");
	}

	_epollFd = epoll_create(5);
	if (_epollFd == -1) {
		throw std::runtime_error("Failed to create epoll file descriptor");
	}
	memset(&this->_servEpollEvent, '\0', sizeof(struct epoll_event));
	this->_servEpollEvent.events = EPOLLIN;
	this->_servEpollEvent.data.fd = this->_socket;
	epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socket, &this->_servEpollEvent);
	SetMap();

}

Server::~Server()
{}

void Server::SetMap()
{
	_commands["PASS"] = &Server::pass;
	_commands["USER"] = &Server::user;
	_commands["NICK"] = &Server::nick;
	// _commands["KICK"] = &Server::kick;
	// _commands["INVITE"] = &Server::invite;
	// _commands["TOPIC"] = &Server::topic;
	_commands["MODE"] = &Server::mode;
	_commands["PRIVMSG"] = &Server::privmsg;
	_commands["JOIN"] = &Server::join;
	_commands["PART"] = &Server::part;
	// _commands["QUIT"] = &Server::quit;
	// _commands["PING"] = &Server::ping;
	// _commands["PONG"] = &Server::pong;
	// _commands["ERROR"] = &Server::error;
}


void Server::NewConnectionRequest(int fd)
{
	struct sockaddr_in	new_addr;
	int					addrLen = sizeof(new_addr);

	int	clientFd = accept(fd, (struct sockaddr*)&new_addr,
		(socklen_t*)&addrLen);
	std::cerr << "new connection accepted on " << clientFd << std::endl;
	Client *addedClient = new Client(clientFd);
	AddClient(clientFd, addedClient);
}

void Server::CheckConnection(Client *client)
{

	if (client->GetPassword() && !client->GetNickname().empty() && !client->GetUsername().empty())
	{
		client->SetAuthenticate();
//		client->addMessageToSendbox(":irc.localhost 001 " + client->GetUsername() + " :Welcome to the " + "networkName" + " Network, " + client->GetUsername() + "!\r\n");
		NumericReplies::reply::welcome(*client);
client->updateClientStatus(this->_epollFd);
	}
}


void Server::HandleEvent(int fd)
{
	fdClientMap::iterator curClient = _clients.find(fd);
	char buf[BUFFER_READ_SIZE + 1] = {0};
	// std::memset(buf, 0, BUFFER_READ_SIZE + 1);
	ssize_t ret_data = recv(fd, buf, BUFFER_READ_SIZE, 0);
	if (ret_data <= 0)
		throw std::invalid_argument("Client disconnected");
	try{
		curClient->second->handleString(std::string(buf));
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		throw std::runtime_error("");
	}
}

void	Server::sendMsg(int fd)
{
	fdClientMap::iterator client =	this->_clients.find(fd);
	if (client == this->_clients.end()){
		std::cerr << "Client not found, that's a problem" << std::endl;
		return ;
	}
	client->second->receiveMsg();
	client->second->updateClientStatus(this->_epollFd);
}

Server &Server::operator=(const Server &server)
{
	if (this != &server)
	{
		//TODO faire jai la flemme de lefaire maintenant
	}
	return (*this);
}

bool	Server::HandleCommand(std::string const &msg, Client *client)
{
	std::istringstream		SepMsg(msg);
	std::string				pushBackArgs;
	vectorCommand			ParsMsg = ParsCommand(msg);

	try
	{
		Handler	function = _commands.at(ParsMsg[0]);
		(this->*function)(ParsMsg, client);
		if (!client->IsAuthenticate())
			CheckConnection(client);
		ParsMsg.clear();
		if (ParsMsg[0] != "PASS")
			std::cout << client->GetUsername() << ", " << client->GetNickname() << ": " << msg;
		return (false);
	}
	catch (NotAuthenticate &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (NeedMoreParams &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (BadPassword &e)
	{
		throw BadPassword();
	}
	catch (AlreadyRegistred &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (UnableToCreateChannel &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (NotAChannel &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (UserAlreadyExist &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (NickAlreadyExist &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (ChannelNotFound &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (ClientNotFound &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (NotInTheChannel &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	catch (std::runtime_error &e) {
		std::cout << e.what() << std::endl;
		return false;
	}
	catch (std::exception &e)
	{
		while (!ParsMsg.empty())
		{
			std::cout << ParsMsg.back() << std::endl;
			ParsMsg.pop_back();
		}
		std::cout << "404 cmd not found" << std::endl;
		ParsMsg.clear();
		std::cout << client->GetUsername() << ", " << client->GetNickname() << ": " << msg;
		return (true);
	}
	//TODO send numeric reply to client
	return (true);
}

channelMap & Server::GetChannels()
{
	return (_channels);
}

fdClientMap & Server::GetClients()
{
	return (_clients);
}

uint64_t Server::GetPort() const
{
	return (_port);
}

int Server::GetScocket() const
{
	return (_socket);
}

int Server::GetEpollFd() const
{
	return (_epollFd);
}

sockaddr_in Server::GetSockAddr() const
{
	return (_sockaddr);
}


void Server::SetPort(uint64_t port)
{
	_port = port;
}


void Server::AddClient(int key, Client *clientToAdd)
{
	clientToAdd->updateClientStatus(this->_epollFd);
	_clients[key] = clientToAdd;
}

void Server::AddChannel(std::string name, Channel *channel)
{
	_channels[name] = channel;
}

void	Server::RemoveChannel(std::string name)
{
	_channels.erase(name);
}

void Server::RemoveClient(int key)
{
	fdClientMap::iterator toRemove = _clients.find(key);
	if (toRemove == _clients.end())
	{
		std::cout << "Client to remove not found" << std::endl;
		return;
	}
	this->_nickUsed.erase(toRemove->second->GetNickname());
	std::map<vectorCommand , Client *> partToExecute;
	for (channelMap::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second->IsInChannel(toRemove->second->GetNickname()))
		{
			vectorCommand	PartCommand;
			PartCommand.push_back("PART");
			PartCommand.push_back("#" + it->first);
			partToExecute.insert(std::make_pair(PartCommand,toRemove->second));
//			part(PartCommand, toRemove->second);
		}
	}
	//execute all part stored in the partToExecute map.
	for (std::map<vectorCommand , Client *>::iterator it = partToExecute.begin(); it != partToExecute.end(); ++it)
		part(it->first, it->second);
	close(key);
	delete toRemove->second;
	_clients.erase(key);
}

void Server::deleteClient(fdClientMap::iterator toDelete) const {
	epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, toDelete->first, NULL);
	close(toDelete->first);
	delete toDelete->second;
}

void Server::deleteChannel(channelMap::iterator toDelete)
{
	delete toDelete->second;
}


void Server::exitservClean() {
	if (!_clients.empty()){
		for (fdClientMap::iterator cur = _clients.begin(); cur != _clients.end(); cur++)
			deleteClient(cur);
		_clients.clear();
	}
	if (!_channels.empty()){
		for (channelMap::iterator cur = _channels.begin(); cur != _channels.end(); cur++)
			deleteChannel(cur);
		_channels.clear();
	}
	close(this->_epollFd);
	close(this->_socket);
}
