/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOIS.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/06 12:01:00 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::WHOIS(const Client client)
{
	std::string channelList;
	std::map<std::string, Client> &globalUsers = getGlobalUsers();
	std::map<std::string, Client>::iterator it = globalUsers.find(getInput()[1]);
	if (it != globalUsers.end())
	{
		std::map<std::string, Channel> channels = getChannels();
		std::map<std::string, Channel>::iterator it = channels.begin();
		while (it != channels.end())
		{
			std::map<std::string, Client> users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.find(getInput()[1]);
			if (us != users.end())
			{
				if (Utils::isOperator(it->second, getInput()[1]))
					channelList += "@";
				channelList += it->first;
				channelList += " ";
			}
			++it;
		}
		Utils::trim(channelList);
		SEND(client.getSocket(), ":" + getHostname() + " 311 " + client.getNick() + " " + getInput()[1] + " ~" + getInput()[1] + " " + "  FT_IRC_42Porto_v1.0 * :" + getInput()[1] + "\r\n", "Error sending WHOIS message");
		SEND(client.getSocket(), ":" + getHostname() + " 319 " + client.getNick() + " " + getInput()[1] + " :" + channelList + "\r\n", "Error sending WHOIS message");
		SEND(client.getSocket(), ":" + getHostname() + " 312 " + client.getNick() + " " + getInput()[1] + " " + getHostname() + " :is it a Bird, is it Superman? No, it's " + getInput()[1] + "\r\n", "Error sending WHOIS message");
		SEND(client.getSocket(), ":" + getHostname() + " 330 " + client.getNick() + " " + getInput()[1] + " " + getInput()[1] + " :is logged in as" + "\r\n", "Error sending WHOIS message");
		SEND(client.getSocket(), ":" + getHostname() + " 318 " + client.getNick() + " " + getInput()[1] + " :End of /WHOIS list." + "\r\n", "Error sending WHOIS message");
	}
}
