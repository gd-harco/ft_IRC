#include "server.hpp"
#include <unistd.h>
#include <csignal>

Server::Server()
{
}

Server::Server(const Server &server)
{
	*this = server;
}

void	handleSigInt(int sig);

void Server::launchServer(void)
{
	signal(SIGINT, handleSigInt);
	std::cout << "Waiting for incoming connection on port " << this->GetPort() << std::endl;
	struct epoll_event eventsCaught[MAX_EVENT];

	while (true)
		waitLoop(eventsCaught);
}

void Server::waitLoop(struct epoll_event eventsCaught[MAX_EVENT])
{
	int nfds = epoll_wait(this->GetEpollFd(), eventsCaught, MAX_EVENT, -1);
	if (nfds == -1)
	{
		perror("epoll wait");
		exit(1);
	}
	for (int i = 0; i < nfds; ++i) {
		handleEvent(eventsCaught[i]);
	}
}

void Server::handleEvent(struct epoll_event event)
{
	int currFd = event.data.fd;
	if (currFd == this->GetScocket())
	{
		//this mean we got a new incoming connection
		std::cout << "new connection detected" << std::endl;
		this->NewConnectionRequest(currFd);
	}
	else
	{
		handleExistingConnection(event, currFd);
	}
}

void Server::handleExistingConnection(struct epoll_event event, int currFd)
{
	if (event.events & EPOLLOUT)
	{
		this->sendMsg(event.data.fd);
		if (_clients[currFd]->_toDestroy)
			this->RemoveClient(currFd);
	}
	if (event.events & EPOLLIN)
	{
		handleEventEpollin(currFd);
	}
}

void Server::handleEventEpollin(int currFd)
{
	//si le read fail, alors le client a ete deco
	try
	{
		this->HandleEvent(currFd);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		//parcourir les channels pour voir si il y a le fd et le suppr
		this->RemoveClient(currFd);
		epoll_ctl(this->GetEpollFd(), EPOLL_CTL_DEL, currFd, 0);
		close(currFd);
	}
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
	_commands["KICK"] = &Server::kick;
	_commands["INVITE"] = &Server::invite;
	_commands["TOPIC"] = &Server::topic;
	_commands["MODE"] = &Server::mode;
	_commands["PRIVMSG"] = &Server::privmsg;
	_commands["JOIN"] = &Server::join;
	_commands["PART"] = &Server::part;
	 _commands["QUIT"] = &Server::quit;
	_commands["PING"] = &Server::ping;
	_commands["PONG"] = &Server::pong;
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
		NumericReplies::reply::welcome(*client);
client->updateClientStatus(this->_epollFd);
	}
}


void Server::HandleEvent(int fd)
{
	fdClientMap::iterator curClient = _clients.find(fd);
	char buf[BUFFER_READ_SIZE + 1] = {0};
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
	catch (std::exception &e)
	{
		while (!ParsMsg.empty())
		{
			std::cout << ParsMsg.back() << std::endl;
			ParsMsg.pop_back();
		}
		// std::cout << "404 cmd not found" << std::endl;
		ParsMsg.clear();
		std::cout << e.what() << std::endl;
		return (true);
	}
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

Client *Server::findClient(std::string nickName)
{
	for (fdClientMap::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
	    Client* client = it->second;
	    if (client->GetNickname() == nickName)
	        return client;
	}
	return NULL;
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
