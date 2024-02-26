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
		stringClientMap	GetClients() const;
		std::string		GetAllNickname();
		bool			GetRTopic() const;
		const			std::vector<std::string>		&GetOp() const;
		bool			IsInChannel(std::string const &user) const;
  		int				GetUserLimit() const;
		bool			isOp(std::string const &user);
		bool			IsInvite(std::string const &nick);
		bool			GetRInvite() const;
//		std::string		GetName() const;
//		bool			IsRestrictChannel() const;

		//setters
		void	SetPassword(std::string const &NewPassword);
		void	SetTopics(std::string const &NewTopic);
		void	RemoveOp(std::string const &op);
		void	SetInvite(std::string const &nick);
		void	SetRTopic(bool status);
		void	SetOp(std::string const &op);
		void	SetUserLimit(int NewLimit);
		void	SetRInvite(bool status);
//		void	SetName(std::string const &NewName);

		//members functions
		void	AddClient(const std::string& key, int value);
		void	RemoveClient(const std::string& key);

	private:
		stringClientMap				_clients;
		std::string					_password;
		std::string					_topic;
		std::string 				_name;
		std::vector<std::string>	_op;
		std::vector<std::string>	_inviting_users;
		bool						_rTopic;
		bool						_rInvite;
		int							_userLimit;

};

#endif
