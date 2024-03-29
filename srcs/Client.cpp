/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 00:24:23 by ialves-m         ###   ########.fr       */
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

void Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::getClientLoginData(char* buffer, int bytesRead)
{
	std::string message(buffer, bytesRead);
	if (message.substr(0, 12) != "CAP LS 302\r\n")
	{
		setNick(message.substr(message.find("NICK ") + 17, message.find_first_of("\r\n")));
		
		std::cout << getNick() << std::endl;
	}
}