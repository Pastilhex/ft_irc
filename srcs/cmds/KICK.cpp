/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/26 07:30:40 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::KICK(std::string message, Client client)
{
	bool isKickerOp = false;
	bool isKickedOp = false;

	std::string kickNick = getInput()[2];
	std::string reason = (getInput().size() == 4 && !getInput()[3].empty()) ? getInput()[3] : "";
	std::string channelName = getInputCmd(message, "KICK");

	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		{
			std::vector<std::string> &operators = it->second.getOperators();
			std::vector<std::string>::iterator op = operators.begin();
			while (op != operators.end())
			{
				if ("@" + client.getNick() == *op)
					isKickerOp = true;
				++op;
			}
		}
		{
			std::vector<std::string> &operators = it->second.getOperators();
			std::vector<std::string>::iterator op = operators.begin();
			while (op != operators.end())
			{
				if ("@" + kickNick == *op)
					isKickedOp = true;
				++op;
			}
		}

		if (isKickerOp && isKickedOp)
		{
			std::string leaveChannel = ":" + getHostname() + " 481 " + client.getNick() + " " + getInputCmd(message, "KICK") + " :Permission Denied - You're not an IRC operator\r\n";
			SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
			return;
		}
		else if ((!isKickerOp && isKickedOp) || (!isKickerOp && !isKickedOp))
		{
			std::string leaveChannel = ":" + getHostname() + " 482 " + client.getNick() + " " + getInputCmd(message, "KICK") + " :You're not channel operator\r\n";
			SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
			return;
		}
		else if (isKickerOp && !isKickedOp)
		{
			std::map<std::string, Client>& users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->first == kickNick)
				{
					std::string leaveChannel = RPL_KICK(client, channelName, kickNick, reason);
					SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
					broadcastKICK(client, kickNick, channelName, reason);
					std::map<std::string, Client>::iterator tmp = us;
					users.erase(tmp);
					updateChannel(it->second);
					break;
				}
				++us;
			}
		}
	}
}
