#include "headers/channel.hpp"
#include "headers/client.hpp"
#include "headers/server.hpp"
#include <cstdlib>


int	main(int ac, char **av)
{
	Server Server;
	Client	NewClient0("Zz", "zzz");
	Client	NewClient1("aa", "aa");
	Client	NewClient2("bb", "bb");

	Channel	NewChannel0("ca chat ou quoi la0", "couilles0");
	Channel	NewChannel1("ca chat ou quoi la1", "couilles1");
	Channel	NewChannel2("ca chat ou quoi la2", "couilles2");

	if (ac != 2)
	{
		return (1);
	}
	Server.SetPort(std::atoi(av[1]));
	std::cout << Server.GetPort() << std::endl;
	Server.AddClient(10, NewClient0);
	Server.AddClient(11, NewClient1);
	Server.AddClient(12, NewClient2);

	Server.AddChannel(NewChannel0.GetName(), NewChannel0);
	Server.AddChannel(NewChannel1.GetName(), NewChannel1);
	Server.AddChannel(NewChannel2.GetName(), NewChannel2);

	std::map<int, Client>	SaveClients = Server.GetClients();

	for(std::map<int, Client>::iterator it = SaveClients.begin(); it != SaveClients.end(); ++it)
	{
		std::cout << it->first << " => " << SaveClients[it->first].GetNickname() << std::endl;
	}

	std::cout << std:: endl;

	std::map<std::string, Channel>	SaveChannel = Server.GetChannels();

	for(std::map<std::string, Channel>::iterator it = SaveChannel.begin(); it != SaveChannel.end(); ++it)
	{
		std::cout << it->first << " => " << SaveChannel[it->first].GetTopics() << std::endl;
	}

	std::cout << std:: endl;

	Server.RemoveChannel("couilles0");

	SaveChannel = Server.GetChannels();

	for(std::map<std::string, Channel>::iterator it = SaveChannel.begin(); it != SaveChannel.end(); ++it)
	{
		std::cout << it->first << " => " << SaveChannel[it->first].GetTopics() << std::endl;
	}

	Server.RemoveClient(10);

	std::cout << std:: endl;

	SaveClients = Server.GetClients();

	for(std::map<int, Client>::iterator it = SaveClients.begin(); it != SaveClients.end(); ++it)
	{
		std::cout << it->first << " => " << SaveClients[it->first].GetNickname() << std::endl;
	}

	return (0);
}