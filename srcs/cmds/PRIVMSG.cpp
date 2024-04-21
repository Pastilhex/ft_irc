/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/21 10:02:23 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//:Lenovo!ivo@ivo-Yoga-7-14ITL5 PRIVMSG ivo :DCC SEND microshell.c 2130706433 35159 1512
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
			// isDCC_SEND(input[2]);
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

// bool Server::isDCC_SEND(std::string message)
// {
// 	if (message.find("DCC SEND") != std::string::npos)
// 	{
// 		std::vector<std::string> words;
// 		std::string word;
// 		int begin = message.find("\001");
// 		int end = message.find_last_of("\001");
// 		std::string trimmed = message.substr(begin, end - begin + 1);
// 		std::stringstream ss(trimmed);
// 		while (ss >> word)
// 		{
// 			if (word[0] == '\001')
// 			{
// 				words.push_back(word.substr(1));
// 				while (ss >> word)
// 				{
// 					words.back() += " ";
// 					words.back() += word;
// 				}
// 			}
// 			else
// 				words.push_back(word);
// 		}
// 		return true;
// 	}
// 	return false;
// }