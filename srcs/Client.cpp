/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/07 16:40:05 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Client::Client(void) {}

void Client::setNewClient(Client &client)
{
	client._nick.clear();
	client._serverPassword.clear();
	client._tmpPassword.clear();
	client._username.clear();
}

 std::string Client::getNick(void) const
{
	return (this->_nick);
}

std::string Client::getUsername(void)
{
	return this->_username;
}

std::string Client::getTmpPassword(void)
{
	return this->_tmpPassword;
}

void Client::setSocket(int fd)
{
	this->_socket = fd;
}

int Client::getSocket()
{
	return this->_socket;
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

void	Client::getClientLoginData(char* buffer, int bytesRead, std::map<std::string, Client> globalUsers, std::string hostname)
{
	std::string message(buffer, bytesRead);
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
		// if (isCMD(message, "USERHOST"))
		// 	setUsername(getInputCmd(message, "USERHOST"));
		if (isCMD(message, "USER "))
			setUsername(getInputCmd(message, "USER"));
		if (isCMD(message, "PASS"))
			setTmpPassword(getInputCmd(message, "PASS"));
	}
}
