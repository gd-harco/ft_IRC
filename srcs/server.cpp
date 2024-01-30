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

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = this->_socket;
	epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socket, &event);
}

Server::~Server()
{}

void Server::NewConnectionRequest(int fd)
{
	struct sockaddr_in	new_addr;
	int					addrLen = sizeof(new_addr);

	int	client = accept(fd, (struct sockaddr*)&new_addr,
		(socklen_t*)&addrLen);
	Client	NewClient (client, this->_epollFd);
	AddClient(client, NewClient);
}

void Server::HandleMessage(int fd)
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
		//TODO: iterer dans la liste des clients : passer le client en mode EPOLLOUT
		// et ajouter le message a la liste du client
		HandleCommand(msg, _clients[fd]);
		std::cout << _clients[fd].GetUsername() << ", " << _clients[fd].GetNickname() << ": " << "full string : " << msg << std::endl;
		msg.clear();
	}
}

Server &Server::operator=(const Server &server)
{
	if (this != &server)
	{
		// faire jai la flemme de lefaire maintenant
	}
	return (*this);
}

void	Server::HandleCommand(std::string const &msg, Client &client)
{
	std::istringstream	SepMsg(msg);
	std::string			Command;
	std::string			Args;
	(void)client;

	SepMsg >> Command >> Args;

	//TODO send numeric reply to client
	if (Command == "PASS" && Args == _password && client.GetPassword() == false)
		client.SetPassword();
	else if (Command == "NICK")
		client.SetNickname(Args);
	else if (Command == "USER")
		client.SetUsername(Args);
}


//getters
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


void Server::AddClient(int key, Client &client)
{
	_clients.insert(std::make_pair(key, client));
}

void Server::AddChannel(std::string name, Channel &channel)
{
	_channels.insert(std::make_pair(name, channel));
}

void	Server::RemoveChannel(std::string name)
{
	_channels.erase(name);
}

void Server::RemoveClient(int key)
{
	_clients.erase(key);
}

std::string Server::GetPasswod() const
{
	return (_password);
}

