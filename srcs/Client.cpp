/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 14:04:45 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

std::string Client:: getNick(void)
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
			setNick(message.substr(message.find("NICK ") + 5, message.find("\r", message.find("NICK ") + 5) - (message.find("NICK ") + 5)));
		if (message.find("USER") != std::string::npos)
			setUsername(message.substr(message.find("USER ") + 5, message.find(" ", message.find("USER ") + 5) - (message.find("USER ") + 5)));
		if (message.find("PASS") != std::string::npos)
			setTmpPassword(message.substr(message.find("PASS ") + 5, message.find("\r", message.find("PASS ") + 5) - (message.find("PASS ") + 5)));
	}
}