#include "server.hpp"



//TODO mettre chaques fonctions dans son fichier pour pouvoir fair des sous fonction et pas galerer apres pour debug

//TODO check le nombre d'args quand le parsing sera fait

const char *Server::NeedMoreParams::what() const throw()
{
	return ("Server::Commands::NeedMoreParams: Need more params");
}

const char *Server::BadPassword::what() const throw()
{
	return ("Server::Commands::BadPassword: Client have bad password");
}

const char *Server::AlreadyRegistred::what() const throw()
{
	return ("Server::Commands::AlreadyRegistred: Client already regisred");
}

const char *Server::UserAlreadyExist::what() const throw()
{
	return ("Server::Commands::UserAlreadyExist: User already exist");
}

const char *Server::NickAlreadyExist::what() const throw()
{
	return ("Server::Commands::NickAlreadyExist: Nick already exist");
}

const char *Server::NotAuthenticate::what() const throw()
{
	return ("Server::Commands::NotAuthenticate:  User not authenticate");
}

const char *Server::ChannelNotFound::what() const throw()
{
	return ("Server::Commands::ChannelNotFound:  Channel not found");
}

const char *Server::ClientNotFound::what() const throw()
{
	return ("Server::Commands::ClientNotFound:  Client not found");
}

const char *Server::NotAChannel::what() const throw()
{
	return ("Server::Commands::NotAChannel: This is not a channel");
}

const char *Server::NotInTheChannel::what() const throw()
{
	return ("Server::Commands::NotInTheChannel: User is not in the channel");
}

const char *Server::UnableToCreateChannel::what() const throw()
{
	return ("Server::Commands::UnableToCreateChannel:  User cannot create this channel (no #)");
}

const char *Server::ErroneusNickName::what() const throw()
{
	return ("Server::Commands::ErroneusNickName:  erroneus nick name");
}
const char *Server::OperatorIsNeeded::what() const throw()
{
	return ("Server::Commands::OperatorIsNeeded: You're not channel operator");
}

const char *Server::AlreadyOnChannel::what() const throw()
{
	return ("Server::Commands::AlreadyOnChannel: User already on channel");
}

const char *Server::BadPongResponse::what() const throw()
{
	return ("Server::Commands::BadPongResponse: Token received not equel to token send");
}
