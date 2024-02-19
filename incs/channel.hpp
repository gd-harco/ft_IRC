#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <bits/atomic_lockfree_defines.h>

#include "client.hpp"

typedef std::map<std::string, int> stringClientMap;

class	Channel
{
	public:
		//constructor / destructor
		Channel();
		Channel(const std::string &name, const std::string &opName);
		~Channel();

		//getters
		std::string		GetPassword() const;
		std::string		GetTopics() const;
		std::string		GetName() const;
		stringClientMap	GetClients() const;
		std::string		GetAllNickname();
		const std::vector<std::string>		&GetOp() const;
		bool			IsInChannel(std::string const &user) const;
		bool			IsRestrictChannel() const;
  	int       GetUserLimit() const;
		bool			isOp(std::string const &user);

		//setters
		void	SetPassword(std::string const &NewPassword);
		void	SetTopics(std::string const &NewTopic);
		void	SetName(std::string const &NewName);
		void	SetOp(std::string const &op);
		void	RemoveOp(std::string const &op);
		void	SetRTopic(bool status);
		void	SetUserLimit(int NewLimit);
		void	SetRInvite(bool status);

		//members functions
		void	AddClient(std::string key, int value);
		void	RemoveClient(std::string key);

	private:
		stringClientMap				_clients;
		std::string					_password;
		std::string					_topic;
		std::string 				_name;
		std::vector<std::string>	_op;
		bool						_rTopic;
		bool						_rInvite;
		int							_userLimit;

};

#endif
