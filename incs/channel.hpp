#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "client.hpp"

typedef std::map<std::string, int> stringClientMap;

class	Channel
{
	public:
		//constructor / destructor
		Channel();
		Channel(const std::string &name);
		Channel(const std::string &topic, const std::string &name);
		~Channel();

		//getters
		std::string		GetPassword() const;
		std::string		GetTopics() const;
		std::string		GetName() const;
		stringClientMap	GetClients() const;
		bool			GetHavePassword() const;
		std::string		GetAllNickname();

		//setters
		void	SetPassword(std::string &NewPassword);
		void	SetTopics(std::string &NewTopic);
		void	SetName(std::string &NewName);

		//members functions
		void	AddClient(std::string key, int value);
		void	RemoveClient(std::string key);

	private:
		stringClientMap	_clients;
		std::string		_password;
		std::string		_topic;
		std::string 	_name;
		bool			_havePassword;

};

#endif
