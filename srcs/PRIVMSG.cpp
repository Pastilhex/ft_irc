/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 21:50:30 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::PRIVMSG(std::string message, Client client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName = "";
	std::string msgToSend = getMessage(message);

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
	int end = message.find_first_of("\r\n", begin); //find_last_not_of returns the position of the last character that is not in the string
	return message.substr(begin, end - begin);
}