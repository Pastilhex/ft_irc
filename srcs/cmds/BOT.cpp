/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BOT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:33:55 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/24 22:36:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

bool Server::BOT(Client &client, std::vector<std::string> &input)
{
	(void)client;
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName = "";
	std::string msgToSend = getMessage(input[2]);
	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			break;
		}
		++inputIterator;
	}
	Bot::createBot(this, channelName);
	updateChannel(client, channelName);
	SEND(client.getSocket(), msgToSend, "Error sending BOT message");
	return true;
}