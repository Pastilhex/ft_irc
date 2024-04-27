/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/24 22:49:29 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::JOIN(int clientSocket, Client &client)
{
	bool canJoin = true;
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName;
	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			std::map<std::string, Channel> &channels = getChannels();
			std::map<std::string, Channel>::iterator it = channels.find(channelName);

			if (it != channels.end())
			{
				if(it->second.getUsers().size() >= (size_t)it->second.getUserLimit())
				{
					SEND(client.getSocket(), ERR_CHANNELISFULL(client.getNick(), it->first), "Error sending JOIN message with mode +l advise to user");
					canJoin = false;
				}
				std::vector<char> mode = it->second.getModes();
				std::vector<char>::iterator mode_it = mode.begin();
				for (; mode_it != mode.end(); ++mode_it)
				{
					if (*mode_it == 'i')
					{
						if (!isUserInvited(client.getNick(), channelName))
						{
							SEND(client.getSocket(), ERR_INVITEONLYCHAN(client, channelName), "Error sending JOIN message with mode +i advise to user");
							canJoin = false;
						}
					}
					if (*mode_it == 'k')
					{
						if (input[2].empty() || input[2] != getPassword())
						{
							SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error sending JOIN message with mode +k advise to user");
							canJoin = false;
						}
					}
				}
				if (!Utils::isOperator(it->second, client.getNick()) && canJoin)
					it->second.RemoveInvited(client.getNick());
			}
			if (((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&') && canJoin)
			{
				for (it = channels.begin(); it != channels.end(); ++it)
				{
					if (it->first == channelName)
					{
						it->second.setNewUser(client);
						SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
						updateChannel(it->second);
						MODE(client);
						break;
					}
				}
				if (it == channels.end())
				{
					bool state = (input[1][0] == '#') ? false : true;
					Channel channel = Channel(channelName, state);
					channel.setNewUser(client);
					channel.AddOperator(client.getNick());
					setNewChannel(channelName, channel);
					SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
					MODE(client);
					WHO(client.getSocket(), client);
				}
			}
			++inputIterator;
		}
		else
			++inputIterator;
	}
}
