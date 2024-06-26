/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:50:24 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::INVITE(Client client)
{
	bool userAvailable = false;
	int invitedFd;
	std::string invitedUser = getInput()[1];
	std::string channel = getInput()[2];
	if (getInput().size() != 3)
		return;
	const std::map<std::string, Channel>::iterator it = getChannels().find(channel);
	if (!it->second.getInvisibility())
		return;
	if (it == getChannels().end())
		return (SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, channel), "Error sending ERR_NOSUCHCHANNEL (403)"));
	if (!Utils::isValidUser(it->second, client.getNick()))
		return (SEND(client.getSocket(), ERR_NOTONCHANNEL(client.getNick(), channel), "Error sending ERR_NOTONCHANNEL (442)"));
	if (Utils::isValidUser(it->second, invitedUser))
		return (SEND(client.getSocket(), ERR_USERONCHANNEL(client.getNick(), invitedUser, channel), "Error sending ERR_USERONCHANNEL (443)"));
	if (!Utils::isOperator(it->second, client.getNick()))
		return (SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, channel), "Error sending ERR_CHANOPRIVSNEEDED (482)"));
	std::map<std::string, Channel> &channels = this->getChannels();
	std::map<std::string, Channel>::iterator ch = channels.begin();
	while (ch != channels.end())
	{
		if (ch->first == channel)
		{
			std::map<std::string, Client> &users = this->getGlobalUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->first == invitedUser)
				{
					userAvailable = true;
					invitedFd = us->second.getSocket();
					break;
				}
				++us;
			}
			break;
		}
		++ch;
	}
	if (userAvailable)
		ch->second.AddInvited(invitedUser);
	else
		return (SEND(client.getSocket(), ERR_NOSUCHNICK(client, invitedUser), "Error sending msg ERR_NOSUCHNICK"));
	std::string msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " INVITE " + invitedUser + " " + channel + "\r\n";
	SEND(invitedFd, msg, "Error sending INVITE message");
}
