#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include "client.hpp"

class	Channel
{
	public:
		//constructor / destructor
		Channel();
		Channel(const std::string	&topic, const std::string &name);
		~Channel();

		//getters
		std::string	GetPassword() const;
		std::string	GetTopics() const;
		std::string	GetName() const;
		std::map<std::string, int>	GetClients() const;

		//setters
		void	SetPassword(std::string &NewPassword);
		void	SetTopics(std::string &NewTopic);
		void	SetName(std::string &NewName);

		void	AddClient(std::string key, int value);
		void	RemoveClient(std::string key);

	private:
		std::map<std::string, int> _clients;
		std::string	_password;
		std::string	_topic;
		std::string _name;

};

#endif