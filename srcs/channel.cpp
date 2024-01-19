#include "../headers/channel.hpp"

Channel::Channel()
{}

Channel::~Channel()
{}

Channel::Channel(const std::string &topic, const std::string &name): _topic(topic), _name(name)
{}

std::string Channel::GetName() const
{
	return (_name);
}

std::map<std::string, int> Channel::GetClients() const
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

void Channel::SetPassword(std::string &NewPassword)
{
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

// void Channel::AddClient(std::string key, int value)
// {

// }


