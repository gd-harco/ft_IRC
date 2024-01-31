#include "server.hpp"

Server::Server()
{
}

Server::Server(const Server &server)
{
	*this = server;
}

Server::Server(uint64_t port, std::string password)
{
	_port = port;
	_password = password;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		throw std::runtime_error("Failed to create socket");
	}
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
}

Server::~Server()
{}

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

void Server::HandleEvent(int fd)
{
	char buf[BUFFER_READ_SIZE + 1];
	static std::string	msg;
	const std::string delimeter("\r\n");
	std::memset(buf, 0, BUFFER_READ_SIZE + 1);
	ssize_t ret_data = recv(fd, buf, BUFFER_READ_SIZE, 0);
	if (ret_data <= 0)
		throw std::invalid_argument("Client disconnected");
	msg.append(std::string(buf));
	if (msg.find(delimeter) != msg.npos)
	{
		if (HandleCommand(msg, _clients[fd]) == true && _clients[fd]->GetPassword() == true && !_clients[fd]->GetNickname().empty() && !_clients[fd]->GetUsername().empty())
		{
			for (fdClientMap::iterator cur = _clients.begin(); cur != _clients.end(); ++cur){
				if (cur->first == fd)
					continue;
				cur->second->addMessageToSendbox(msg);
				cur->second->updateClientStatus(this->_epollFd);
			}
		}
		std::cout << _clients[fd]->GetUsername() << ", " << _clients[fd]->GetNickname() << ": " << "full string : " << msg;
		msg.clear();
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
		// faire jai la flemme de lefaire maintenant
	}
	return (*this);
}

bool	Server::HandleCommand(std::string const &msg, Client *client)
{
	std::istringstream	SepMsg(msg);
	std::string			Command;
	std::string			Args;
	(void)client;

	SepMsg >> Command >> Args;

	//TODO send numeric reply to client
	if (Command == "PASS" &&  client->GetPassword() == false)
	{
		if (Args == _password && client->GetPassword() == false)
		{
			std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have right password" << std::endl;
			client->SetPassword();
			return false;
		}
		else if (Args != _password && client->GetPassword() == false)
		{
			std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have bad password" << std::endl;
			return false;
		}
		else if (client->GetPassword() == true)
		{
			std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have already right password" << std::endl;
			return false;
		}
	}
	else if (Command == "NICK")
	{
		client->SetNickname(Args);
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new nickname" << std::endl;
		return false;
	}
	else if (Command == "USER")
	{
		client->SetUsername(Args);
		std::cout << client->GetNickname() << " " << client->GetUsername() << " " << client->GetFd() << ": " << "have new username" << std::endl;
		return false;
	}
	return true;
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

void Server::AddChannel(std::string name, Channel channel)
{
	_channels.insert(std::make_pair(name, channel));
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
	delete toRemove->second;
	_clients.erase(key);
}
