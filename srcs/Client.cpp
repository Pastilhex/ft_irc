/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/07 11:07:28 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Client::Client(void) {
	for (int i = 0; i < 2048; i++)
		this->_buffer[i] = '\0';
}

Client::~Client(void) {}

void Client::setNewClient(Client &client)
{
	client._nick.clear();
	client._serverPassword.clear();
	client._tmpPassword.clear();
	client._username.clear();
	client._realname.clear();
}

char *Client::getBuffer(void)
{
	return this->_buffer;
}

void Client::setBuffer(char *buffer)
{
	if (buffer == NULL)
	{
		if (this->_buffer[0] != '\0')
			memset(this->_buffer, 0, sizeof(this->_buffer));
	}
	else
		memcpy(this->_buffer, buffer, strlen(buffer));
}

bool Client::getStatus(void)
{
	return this->_status;
}

void Client::setStatus(bool status)
{
	this->_status = status;
}

std::string Client::getRealName(void) const
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
	return this->_clientPoll;
}

void Client::setSocket(int fd)
{
	this->_clientPoll.fd = fd;
}

void Client::setRealName(std::string realname)
{
	this->_realname = realname;
}

int Client::getSocket(void) const
{
	return this->_clientPoll.fd;
}

void Client::setPoll_fd(int fd)
{
	this->_clientPoll.fd = fd;
}

void Client::setPoll_events(void)
{
	this->_clientPoll.events = POLL_IN;
}

void Client::setPoll_revents(void)
{
	this->_clientPoll.revents = 0;
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

void Client::getClientLoginData(Server server, std::string message, std::map<std::string, Client> globalUsers, std::string hostname)
{
	if (isCMD(message, "NICK") || isCMD(message, "USER") || isCMD(message, "PASS"))
	{
		if (server.getInput().size() >= 1 && server.getInput()[0] == "NICK")
		{
			if (server.getInput().size() == 2)
			{
				std::string nickname = server.getInput()[1];
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
			else
				SEND(this->getSocket(), ERR_NONICKNAMEGIVEN("Error", server), "Error sending login msg");
		}

		if (server.getInput().size() >= 1 && server.getInput()[0] == "USER")
		{
			if (server.getInput().size() >= 4 && !server.getInput()[1].empty() && !server.getInput()[2].empty() && !server.getInput()[3].empty())
				setUsername(server.getInput()[1]);
			else
				SEND(this->getSocket(), "Error: USER <username> <hostname> <servername> :<Real Name>\r\n", "Error sending login msg");

			if (server.getInput().size() == 5 && !server.getInput()[4].empty())
				setRealName(server.getInput()[4]);
		}
		if (server.getInput().size() == 1 && server.getInput()[0] == "PASS")
			SEND(this->getSocket(), (server.getHostname() + " 461 " + this->getNick() + " PASS :Not enough parameters.\r\n"), "Error sending login msg");
		else if (server.getInput().size() >= 1 && server.getInput()[0] == "PASS")
			setTmpPassword(server.getInput()[1]);
	}
}
