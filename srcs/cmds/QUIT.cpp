/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/01 17:24:51 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::QUIT(std::vector<pollfd> fds, int i, const Client client)
{
	std::cout << "Connection closed by the client." << std::endl;
	std::map<std::string, Channel> &channels = this->getChannels();
	std::map<std::string, Channel>::iterator ch = channels.begin();

	std::map<std::string, Client> &globalUsers = this->getGlobalUsers();
	std::map<std::string, Client>::iterator gb = globalUsers.begin();
	while (gb != globalUsers.end())
	{
		if (gb->first == client.getNick())
			globalUsers.erase(gb++);
		else
			++gb;
	}

	while (ch != channels.end())
	{
		std::map<std::string, Client> &users = ch->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		std::string reason = (this->getInput().size() > 1) ? this->getInput()[1] : "Quited from server.";
		while (us != users.end())
		{
			if (us->first == client.getNick())
				users.erase(us++);
			else if (us->second.getNick() != client.getNick() && us->second.getRealName() != "Bot")
			{
				SEND(us->second.getSocket(), RPL_QUIT(client.getNick(), client.getUsername(), reason), "Error sending QUIT message");
				us++;
			}
			else
				++us;
		}

		std::vector<std::string> &operators = ch->second.getOperators();
		std::vector<std::string>::iterator op = operators.begin();
		while (op != operators.end())
		{
			if (*op == client.getNick())
				operators.erase(op++);
			else
				++op;
		}

		std::vector<std::string> &invitedUsers = ch->second.getInvitedUsers();
		std::vector<std::string>::iterator in = invitedUsers.begin();
		while (in != invitedUsers.end())
		{
			if (*in == client.getNick())
				invitedUsers.erase(in++);
			else
				++in;
		}
		++ch;
	}
	close(fds[i].fd);
	fds.erase(fds.begin() + i);
	--i;
}
