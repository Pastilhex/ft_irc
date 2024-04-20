/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/20 13:59:01 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::PART(std::string message, Client &client)
{
	std::string channelName = getInputCmd(message, "PART");
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getNick() == client.getNick())
			{
				SEND(client.getSocket(), RPL_PART(channelName, getMsgToSend(message)), "Erro ao enviar mensagem de saída de canal.");
				users.erase(us);
				if (it->second.getNbrUsers() == 0)
				{
					std::string closeChannel = ":" + getHostname() + " PART " + getInputCmd(message, "PART");
					if (send(client.getSocket(), closeChannel.c_str(), closeChannel.length(), 0) == -1)
					{
						std::cerr << "Erro ao enviar mensagem de fecho de canal." << std::endl;
					}
					else
					{
						channels.erase(it);
						std::cout << closeChannel << std::endl;
					}
				}
				else
					updateChannel(client, channelName);
				break;
			}
			++us;
		}
	}
	else
	{
		std::cout << "Canal não encontrado." << std::endl;
	}
}
