/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/01 17:24:27 by lpicoli-         ###   ########.fr       */
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
		Bot::sendGoodbye(*this, it->second, client);
		while (us != users.end())
		{
			std::string reason = (this->getInput().size() > 2) ? this->getInput()[2] : "Leaving";
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), RPL_PART(channelName, reason), "Erro ao enviar mensagem de saída de canal.");
			std::map<std::string, Client>::iterator tmp = us;
			++us;
			if (client.getNick() == tmp->first)
				users.erase(tmp);
		}
		if (it->second.getNbrUsers() == 0)
			channels.erase(it);
		else
			updateChannel(it->second);
	}
}
