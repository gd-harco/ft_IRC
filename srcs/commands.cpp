#include "client.hpp"

// bool Client::error(Channel &channel)
// {
// }


// bool Client::pong(Channel &channel)
// {
// }


// bool Client::ping(Channel &channel)
// {
// }


// bool Client::quit(Channel &channel)
// {
// }


bool Client::join(Channel &channel)
{
	channel.AddClient(_username, _fd);
	return (true);
}


// bool Client::privmsg(const std::string &msg, Channel &channel)
// {
// }


// bool Client::mode(const unsigned char type, const std::string &Channel)
// {
// }


// bool Client::topic(const std::string &Channel)
// {
// }


// bool Client::invite(const std::string &InvitUser)
// {
// }


// bool Client::kick(const std::string &KickUser)
// {
// }


