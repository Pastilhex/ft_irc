/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/04 05:43:36 by ialves-m         ###   ########.fr       */
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

void	Client::getClientLoginData(char* buffer, int bytesRead)
{
	std::string message(buffer, bytesRead);
	if ((message.find("NICK") != std::string::npos) || (message.find("USER") != std::string::npos) || (message.find("PASS") != std::string::npos))
	{
		if (message.find("NICK") != std::string::npos)
			setNick(message.substr(message.find("NICK ") + 5, message.find_first_of(" \n\r\t", message.find("NICK ") + 5) - (message.find("NICK ") + 5)));
		if (message.find("USER") != std::string::npos)
			setUsername(message.substr(message.find("USER ") + 5, message.find_first_of(" \n\r\t", message.find("USER ") + 5) - (message.find("USER ") + 5)));
		if (message.find("PASS") != std::string::npos)
			setTmpPassword(message.substr(message.find("PASS ") + 5, message.find_first_of(" \n\r\t", message.find("PASS ") + 5) - (message.find("PASS ") + 5)));
	}
}