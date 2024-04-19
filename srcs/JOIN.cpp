/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 12:34:28 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::JOIN(int clientSocket, Client &client)
{
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
					return (SEND(client.getSocket(), ERR_CHANNELISFULL(client.getNick(), it->first), "Error sending JOIN message with mode +l advise to user"));
				std::vector<char> mode = it->second.getModes();
				std::vector<char>::iterator mode_it = mode.begin();
				for (; mode_it != mode.end(); ++mode_it)
				{
					if (*mode_it == 'i')
					{
						if (isUserInvited(client.getNick(), channelName))
						{
							if (!Utils::isOperator(it->second, client.getNick())) // operadores nunca saem da lista de convidados do canal
								it->second.RemoveInvited(client.getNick());
							continue;
						}
						else
						{
							return (SEND(client.getSocket(), ERR_INVITEONLYCHAN(client, channelName), "Error sending JOIN message with mode +i advise to user"));
						}
					}
					if (*mode_it == 'k')
					{
						if (input[2].empty() || input[2] != getPassword())
						{
							SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error sending JOIN message with mode +k advise to user");
							return;
						}
					}
				}
			}
			if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
			{
				for (it = channels.begin(); it != channels.end(); ++it)
				{
					if (it->first == channelName)
					{
						it->second.setNewUser(client);
						SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
						updateChannel(client, channelName);
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
					MODE("MODE", client);
					WHO(client.getSocket(), client);
				}
			}
			++inputIterator;
		}
		else
			++inputIterator;
	}
}
