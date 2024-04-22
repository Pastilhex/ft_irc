/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/20 13:58:42 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::INVITE(Client client)
{
	bool userAvailable = false;
	int invitedFd;
	std::string invitedUser = getInput()[1];
	std::string channel = getInput()[2];

	std::cout << "INVITE command" << std::endl;
	std::cout << "Invited user: " << invitedUser << std::endl;

	if (getInput().size() != 3)
		return;

	const std::map<std::string, Channel>::iterator it = getChannels().find(channel);

	if(!it->second.getInvisibility() && invitedUser != this->getBot().getNick())
		return;

	if(it == getChannels().end())
	{
		SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, channel), "Error sending ERR_NOSUCHCHANNEL (403)");
		return;
	}

	if (!Utils::isValidUser(it->second, client.getNick())) //verifica se o usuario que esta convidando o outro está no canal
	{
		SEND(client.getSocket(), ERR_NOTONCHANNEL(client.getNick(), channel), "Error sending ERR_NOTONCHANNEL (442)");
		return;
	}

	if(Utils::isValidUser(it->second, invitedUser))
	{
		SEND(client.getSocket(), ERR_USERONCHANNEL(client.getNick(), invitedUser, channel), "Error sending ERR_USERONCHANNEL (443)");
		return;
	}

	if (!Utils::isOperator(it->second, client.getNick()))
	{
		SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, channel), "Error sending ERR_CHANOPRIVSNEEDED (482)");
		return;
	}

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
	{
		ch->second.AddInvited(invitedUser);
		if(invitedUser == this->getBot().getNick())
		{
			Client bot = this->getBot();
			it->second.setNewUser(bot); // Adicione esta linha se 'setNewUser' não estiver funcionando
			std::cout << bot.getNick() << " entrou no canal " << it->first << std::endl;
			SEND(client.getSocket(), RPL_JOIN(bot, it->first), "Erro ao entrar no canal.");
			MODE(bot);
			WHO(bot.getSocket(), client);
			std::cout << client.getSocket() << std::endl;
			std::cout << bot.getSocket() << std::endl;
			return;
		}
	}
	else
	{
		SEND(client.getSocket(), ERR_NOSUCHNICK(client, invitedUser), "Error sending msg ERR_NOSUCHNICK");
		return;
	}

	std::string msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " INVITE " + invitedUser + " " + channel + "\r\n";
	SEND(invitedFd, msg, "Error sending INVITE message");
}
