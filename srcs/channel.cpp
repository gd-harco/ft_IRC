#include "channel.hpp"

Channel::Channel()
{}

Channel::~Channel()
{}

Channel::Channel(const std::string &name, const std::string &opName): _name(name), _havePassword(false)
{
	this->SetOp(opName);
}

std::string Channel::GetName() const
{
	return (_name);
}

stringClientMap Channel::GetClients() const
{
	return (_clients);
}

std::string Channel::GetPassword() const
{
	return (_password);
}

std::string Channel::GetTopics() const
{
	return (_topic);
}

bool Channel::GetHavePassword() const
{
	return (_havePassword);
}


void Channel::SetPassword(std::string &NewPassword)
{
	_havePassword = true;
	_password = NewPassword;
}

void Channel::SetTopics(std::string &NewTopic)
{
	_topic = NewTopic;
}

void Channel::SetName(std::string &NewName)
{
	_name = NewName;
}

void Channel::AddClient(std::string key, int value)
{
	_clients.insert(std::make_pair(key, value));
}

void Channel::RemoveClient(std::string key)
{
	std::vector<std::string>::iterator	ToFind = std::find(_op.begin(), _op.end(), key);
	if (ToFind != _op.end())
		_op.erase(ToFind);
	_clients.erase(key);
}

std::string Channel::GetAllNickname()
{
	std::ostringstream	str;
	for (stringClientMap::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (std::find(_op.begin(), _op.end(), it->first) != _op.end())
			str << " @" << it->first;
		else
			str << " " << it->first;
	}
	return (str.str());
}

const std::vector<std::string> &Channel::GetOp() const
{
	return (_op);
}

void Channel::SetOp(std::string const &op)
{
	_op.push_back(op);
}

void Channel::RemoveOp(std::string const &op)
{
	std::vector<std::string>::iterator const it = std::find(_op.begin(), _op.end(), op);
	if (it != _op.end())
		_op.erase(it);
}


bool Channel::IsInChannel(std::string const &user) const
{
	if (_clients.find(user) != _clients.end())
		return (true);
	return (false);
}

void Channel::SetRTopic(bool status)
{
	_rTopic = status;
}

bool Channel::IsRestrictChannel() const
{
	return _rTopic;
}

