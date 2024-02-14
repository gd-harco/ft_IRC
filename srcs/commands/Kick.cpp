#include "server.hpp"

void Server::kick(vectorCommand args, Client *client)
{
    (void) client;
	try
	{
        if (args.size() < 3)
            throw NeedMoreParams();
        std::string channel = args[1];
        if (_channels.find(channel) == _channels.end())
        {
            throw NotAChannel();
        }
        for (int i = 0; i < _channels.find(channel).GetOp().size(); i++)
        {
            std::cout << "[" << i << "]: " << _channels.find(channel).GetOp()[i] << std::endl;
        }
        for (size_t i = 0; i < args.size(); i++)
            std::cout << "args[" << i << "]: " << args[i] << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
