/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/24 22:28:26 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::PRIVMSG(std::string message, Client client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName = "";
	std::string msgToSend = getMessage(message);

	if (msgToSend[0] && msgToSend[0] == '!' && BOT(client, input))
		return ;
	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			break;
		}
		++inputIterator;
	}

	if (channelName[0] != '#' && channelName[0] != '&')
	{
		std::map<std::string, Client> &users = getGlobalUsers();
		std::map<std::string, Client>::iterator user_it = users.find(input[1]);
		if (user_it != users.end())
		{
			if (!isDCC_SEND(input[2], client.getNick()))
				return;
			SEND(user_it->second.getSocket(), RPL_PRIVMSG(channelName, msgToSend), "Error sending message to user.");
		}
		else
			SEND(client.getSocket(), ERR_NOSUCHNICK(client, input[1]), "Error sending message to user.");
	}
	else
	{
		std::map<std::string, Channel> channels = getChannels();
		std::map<std::string, Channel>::iterator it = channels.find(channelName);
		if (it != channels.end())
		{
			std::map<std::string, Client> &users = it->second.getUsers();
			std::map<std::string, Client>::iterator user_it = users.begin();
			while (user_it != users.end())
			{
				if (user_it->first != client.getNick())
				{
					SEND(user_it->second.getSocket(), RPL_PRIVMSG(channelName, msgToSend), "Error sending message to channel.");
				}
				++user_it;
			}
		}
	}
}

std::string getMessage(std::string message)
{
	int begin = message.find_first_of(":") + 1;
	int end = message.find_first_of("\r\n", begin);
	return message.substr(begin, end - begin);
}

bool Server::isDCC_SEND(std::string message, std::string sender)
{
	if (message.find("DCC SEND") != std::string::npos)
	{
		int begin = message.find('\001');
		int end = message.find_last_of('\001');
		std::vector<std::string> words;
		std::string word;
		std::string trimmed = message.substr(begin, end - begin + 1);
		std::stringstream ss(trimmed);
		int i = 0;
		while (ss >> word)
		{
			words.push_back(word.substr(1));
			while (ss >> word)
			{
				if (word[i = word.find('\001')] == '\001')
					word = word.substr(0, i);
				words.push_back(word);
			}
		}
		std::string file = words[2];
		std::string ip = words[3];
		std::string port = words[4];
		if (words[0] != "DCC" && words[1] != "SEND" && !isValidIPAddress(ip) && !isValidPort(port))
			return false;
		DCC_ACCEPT(sender, file, ip, port);
		return true;
	}
	return false;
}

void Server::DCC_ACCEPT(std::string sender, std::string file, std::string ip, std::string port)
{
	std::map<std::string, Client> &users = getGlobalUsers();
	std::map<std::string, Client>::iterator user_it = users.find(sender);
	SEND(user_it->second.getSocket(), RPL_DCC_ACCEPT(sender, file, ip, port), "Error sending DCC ACCEPT message to user.");
}

bool Server::isValidIPAddress(std::string word)
{
	char *endptr;
	unsigned int ip = static_cast<unsigned int>(std::strtoul(word.c_str(), &endptr, 10));
	std::ostringstream oss;
	oss << ((ip >> 24) & 0xFF) << '.' << ((ip >> 16) & 0xFF) << '.' << ((ip >> 8) & 0xFF) << '.' << (ip & 0xFF);
	std::string ipString = oss.str();
	std::istringstream iss(ipString);
	int octet;
	char dot;
	for (int i = 0; i < 4; ++i)
	{
		iss >> octet >> dot;
		if (octet < 0 || octet > 255 || dot != '.')
		{
			return false;
		}
	}
	char c;
	if (iss >> c)
	{
		return false;
	}
	return true;
}

bool Server::isValidPort(std::string word)
{
	char *endptr;
	unsigned int port = static_cast<unsigned int>(std::strtoul(word.c_str(), &endptr, 10));
	return port == 6667;
}
