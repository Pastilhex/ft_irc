/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 05:11:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/09 07:34:26 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::login(Client &client, std::vector<std::string> splitMessage)
{
	{
		client.getClientLoginData(*this, getGlobalUsers());
		std::map<std::string, Client> &globalUsers = getGlobalUsers();
		std::map<std::string, Client>::iterator it = globalUsers.begin();
		while (it != globalUsers.end())
		{
			if (client.getClientPoll().fd == it->second.getClientPoll().fd)
			{
				if (!client.getUsername().empty() && client.getUsername() != it->second.getUsername())
					it->second.setUsername(client.getUsername());

				if (!client.getRealName().empty() && client.getRealName() != it->second.getRealName())
					it->second.setRealName(client.getRealName());

				if (!client.getTmpPassword().empty() && client.getTmpPassword() != it->second.getTmpPassword())
					it->second.setTmpPassword(client.getTmpPassword());

				if (client.getTmpPassword() == this->getPassword())
				{
					if (!client.getNick().empty() && !client.getUsername().empty() && it->second.getStatus() == false)
						sendWelcome(client.getSocket(), client);
				}

				if (!it->second.getNick().empty() && !it->second.getUsername().empty() && !it->second.getTmpPassword().empty() && it->second.getStatus() == false)
				{
					if (it->second.getTmpPassword() != this->getPassword())
					{
						std::cout << "<< " + splitMessage[0] << std::endl;
						SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error while login");
					}
					else
					{
						it->second.setStatus(true);
						this->_globalUsers.insert(std::make_pair(client.getNick(), client));
						this->_globalUsers.erase(it);
						break;
					}
				}
			}
			it++;
		}
	}

	if (getInput().size() == 2 && getInput()[0] == "NICK" && client.getStatus() == true)
	{
		if (client.getNick() != getInput()[1])
		{
			std::string oldNick = client.getNick();
			client.setNick(getInput()[1]);
			std::map<std::string, Client> &globalUsers = getGlobalUsers();
			std::map<std::string, Client>::iterator it = globalUsers.find(oldNick);
			if (it != globalUsers.end())
			{
				std::map<std::string, Channel> &channels = getChannels();
				std::map<std::string, Channel>::iterator ch = channels.begin();
				while (ch != channels.end())
				{
					std::map<std::string, Client> &users = ch->second.getUsers();
					std::map<std::string, Client>::iterator us = users.find(oldNick);
					if (us != users.end())
					{
						users.erase(us);
						users.insert(std::make_pair(client.getNick(), client));
					}

					std::string opNick = "@" + oldNick;
					std::vector<std::string> &operators = ch->second.getOperators();
					std::vector<std::string>::iterator op = find(operators.begin(), operators.end(), opNick);
					if (op != operators.end())
					{
						*op = "@" + getInput()[1];
					}
					setInput(ch->first + " " + getInput()[1]);
					updateChannel(ch->second);
					++ch;
				}
				globalUsers.erase(it);
				globalUsers.insert(std::make_pair(client.getNick(), client));
			}
			SEND(client.getSocket(), (":" + oldNick + "!" + client.getUsername() + "@" + getHostname() + " " + " NICK :" + client.getNick() + "\r\n"), "Erro changing Nick");
		}
	}
	setInput(splitMessage[0]);
}
