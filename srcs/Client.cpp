/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/24 21:30:15 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Client::Client(void) {}
Client::~Client(void){}

void Client::setNewClient(Client &client)
{
	client._nick.clear();
	client._serverPassword.clear();
	client._tmpPassword.clear();
	client._username.clear();
	client._realname.clear();
}

std::string Client::getRealName(void)
{
	return this->_realname;
}

 std::string Client::getNick(void) const
{
	return (this->_nick);
}

std::string Client::getUsername(void) const
{
	return this->_username;
}

std::string Client::getTmpPassword(void)
{
	return this->_tmpPassword;
}

pollfd Client::getClientPoll(void)
{
	return this->clientPoll;
}


void Client::setSocket(int fd)
{
	this->clientPoll.fd = fd;
}

int Client::getSocket(void) const
{
	return this->clientPoll.fd;
}

void Client::setPoll_fd(int fd)
{
	this->clientPoll.fd = fd;
}

void Client::setPoll_events(void)
{
	this->clientPoll.events = POLL_IN;
}

void Client::setPoll_revents(void)
{
	this->clientPoll.revents = 0;
}

void Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void Client::setUsername(std::string username)
{
	this->_username = username;
}

void Client::setTmpPassword(std::string pass)
{
	this->_tmpPassword = pass;
}

void	Client::getClientLoginData(std::string message , std::map<std::string, Client> globalUsers, std::string hostname)
{
	if (isCMD(message, "NICK") || isCMD(message, "USER") || isCMD(message, "PASS"))
	{
		if (isCMD(message, "NICK"))
		{
			std::string nickname = getInputCmd(message, "NICK");
			std::map<std::string, Client>::iterator gb = globalUsers.find(nickname);
			if (gb != globalUsers.end())
				SEND(getSocket(), ERR_NICKNAMEINUSE(hostname, nickname), "Error while getting nickname");
			else
			{
				std::string oldNick = getNick(); 
				globalUsers.erase(oldNick);
				setNick(nickname);
			}
		}
		if (isCMD(message, "USER "))
			setUsername(getInputCmd(message, "USER"));
		if (isCMD(message, "PASS"))
			setTmpPassword(getInputCmd(message, "PASS"));
	}
}
