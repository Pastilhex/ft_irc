/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:41:57 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/08 20:50:00 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Channel::Channel(std::string name, bool isPrivate)
{
	this->_name = name;
	this->_isPrivate = isPrivate;
	this->_isInviteOnly = false;
	this->_restrictedTopic = true;
	this->_userLimit = UNLIMITED_USERS;
	this->_modes.push_back('n');
	this->_modes.push_back('t');
	this->_creationTime = time(0);
}

std::string Channel::getCreationTime(void)
{
	time(&this->_creationTime);

	std::ostringstream oss;
	oss << this->_creationTime;
	std::string strTime = oss.str();
	return strTime;
}

int Channel::getNbrUsers(void)
{
	return this->_clients.size();
}

std::map<std::string, Client> &Channel::getUsers(void)
{
	return this->_clients;
}

std::vector<std::string> &Channel::getOperators(void)
{
	return this->_operators;
}

std::string Channel::getName(void)
{
	return this->_name;
}

std::string Channel::getTopic(void)
{
	return this->_topic;
}

std::string Channel::getPassword(void)
{
	return this->_password;
}

bool Channel::getInvisibility(void)
{
	return this->_isInviteOnly;
}

bool Channel::getRestrictedTopic(void)
{
	return this->_restrictedTopic;
}

bool Channel::getModeRestrictedAccess(void)
{
	return this->_isPrivate;
}

int Channel::getUserLimit(void)
{
	return this->_userLimit;
}

std::vector<char> Channel::getModes(void)
{
	return this->_modes;
}

bool Channel::isNewMode(char mode)
{
	std::vector<char> modes = this->getModes();
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i] == mode)
			return false;
	}
	return true;
}

std::vector<std::string> &Channel::getInvitedUsers(void)
{
	return this->_invited;
}

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void Channel::setInvisibility(bool mode)
{
	this->_isInviteOnly = mode;
}

void Channel::setModeRestrictedAccess(bool mode)
{
	this->_isPrivate = mode;
}

bool Channel::botExists()
{
	std::map<std::string, Client>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (it->second.getRealName() == "Bot")
			return true;
		++it;
	}
	return false;
}

void Channel::setNewUser(Client client)
{
	this->_clients.insert(std::make_pair(client.getNick(), client));
}

void Channel::setNewMode(char mode)
{
	this->_modes.push_back(mode);
}

void Channel::setUserLimit(int limit)
{
	this->_userLimit = limit;
}

void Channel::setPassword(std::string password)
{
	this->_password = password;
}

void Channel::setRestrictedTopic(bool mode)
{
	this->_restrictedTopic = mode;
}

void Channel::setBot(Client bot)
{
	this->_bot = bot;
}

Client Channel::getBot(void)
{
	return this->_bot;
}

void Channel::AddOperator(const std::string &nickname)
{
	std::vector<std::string>::iterator it;
	bool Op_flag = false;

	for (it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{
		if (*it == nickname)
		{
			Op_flag = true;
		}
	}
	if (!Op_flag)
		this->_operators.push_back("@" + nickname);

	if (_isInviteOnly)
		this->AddInvited(nickname);
}

void Channel::AddInvited(const std::string &nickname)
{
	std::vector<std::string>::iterator it;

	for (it = this->_invited.begin(); it != this->_invited.end(); ++it)
	{
		if (*it == nickname)
		{
			return;
		}
	}
	this->_invited.push_back(nickname);
}

void Channel::RemoveOperator(std::string nickname)
{
	std::vector<std::string>::iterator it = this->_operators.begin();
	while (it != this->_operators.end())
	{
		if (*it == "@" + nickname)
		{
			it = this->_operators.erase(it);
			return;
		}
		else
			++it;
	}
	if (_isInviteOnly)
		this->RemoveInvited(nickname);
}

void Channel::RemoveInvited(std::string nickname)
{
	std::vector<std::string>::iterator it = this->_invited.begin();
	while (it != this->_invited.end())
	{
		if (*it == nickname)
		{
			it = this->_invited.erase(it);
			return;
		}
		else
			++it;
	}
}

void Channel::deleteMode(char mode)
{
	for (size_t i = 0; i < _modes.size(); i++)
	{
		if (_modes[i] == mode)
		{
			_modes.erase(_modes.begin() + i);
			i--;
		}
	}
}
