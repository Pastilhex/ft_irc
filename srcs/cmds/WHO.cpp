/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/20 13:59:13 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::WHO(int clientSocket, const Client client)
{
	(void)clientSocket;
	std::vector<std::string> input = getInput();
	std::string channelName;
	for (size_t i = 0; i < getInput().size(); ++i)
	{
		if (input[i][0] == '#' || input[i][0] == '&')
		{
			channelName = input[i];
			break;
		}
	}
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::string listUsers;
		const std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::const_iterator user_it = users.begin();
		while (user_it != users.end())
		{
			std::string nickname = user_it->first;
			std::vector<std::string> opList = it->second.getOperators();
			std::vector<std::string>::iterator op_it = opList.begin();
			while (op_it != opList.end())
			{
				if ((*op_it).find(nickname, 1) != std::string::npos)
				{
					nickname = *op_it;
					break; // Interrompe o loop assim que encontrar uma correspondência
				}
				++op_it;
			}
			listUsers += nickname;
			if (++user_it != users.end())
				listUsers += " ";
		}
		SEND(client.getSocket(), RPL_NAMREPLY(client, channelName, listUsers), "Error sending WHO");
		SEND(client.getSocket(), RPL_ENDOFNAMES(client, channelName), "Error sending WHO");
	}
}
