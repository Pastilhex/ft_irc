/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/08 20:53:47 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::JOIN(int clientSocket, Client &client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();

	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			std::string channelName = *inputIterator;
			joinChannel(clientSocket, client, channelName);
		}
		++inputIterator;
	}
}

void Server::joinChannel(int clientSocket, Client &client, std::string &channelName)
{
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);

	if (it != channels.end())
	{
		if (isChannelFull(it->second))
		{
			SEND(client.getSocket(), ERR_CHANNELISFULL(client.getNick(), it->first), "Error sending JOIN message with mode +l advise to user");
			return;
		}

		if (!canJoinChannel(client, it->second))
			return;

		it->second.setNewUser(client);
		SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
		if (it->second.botExists())
			Bot::sendWelcome(*this, it->second, client);
		updateChannel(it->second);
		MODE(client);
	}
	else
	{
		bool state = (channelName[0] == '#') ? false : true;
		Channel channel = Channel(channelName, state);
		if (state == true)
		{
			channel.setNewMode('i');
			channel.setInvisibility(true);
		}
		channel.setNewUser(client);
		channel.AddOperator(client.getNick());
		setNewChannel(channelName, channel);
		SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
		MODE(client);
		WHO(clientSocket, client);
	}
}

bool Server::isChannelFull(Channel &channel)
{
	return (channel.getUsers().size() >= static_cast<std::map<std::string, Client>::size_type>(channel.getUserLimit()));
}

bool Server::canJoinChannel(const Client &client, Channel &channel)
{
	std::vector<char> mode = channel.getModes();
	std::vector<char>::iterator mode_it = mode.begin();

	for (; mode_it != mode.end(); ++mode_it)
	{
		if (*mode_it == 'i')
		{
			if (!isUserInvited(client.getNick(), channel.getName()))
			{
				SEND(client.getSocket(), ERR_INVITEONLYCHAN(client, channel.getName()), "Error sending JOIN message with mode +i advise to user");
				return false;
			}
		}
		if (*mode_it == 'k')
		{
			std::vector<std::string> input = getInput();
			if (input.size() < 3 || input[2] != getPassword())
			{
				SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error sending JOIN message with mode +k advise to user");
				return false;
			}
		}
	}

	return true;
}
