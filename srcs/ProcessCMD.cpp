/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/06 11:57:02 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::processCMD(Client &client, std::vector<pollfd> &fds, std::string message, size_t &i)
{
	std::vector<std::string> splitMessage = Utils::splitVector(message, "\n");
	while (splitMessage.size())
	{
		if (message.empty() || message.compare("\n") == 0)
			return;
		else
		{
			setInput(splitMessage[0]);
			if (getInput().size() >= 1)
			{
				if (!(getInput()[0] == "PING"))
					std::cout << RED << "<< " << RED + splitMessage[0] << RESET << std::endl;
				
				if (splitMessage[0].find("CAP LS") != std::string::npos)
					SEND(client.getSocket(), ":* CAP * LS :42Porto Ft_IRCv1.0\r\n", "Error sending CAP LS message to client");
				
				else if (splitMessage[0].find("CAP END") != std::string::npos)
					SEND(fds[i].fd, ":* CAP * END\r\n", "Error sending CAP LS message to client");
				
				else if (getInput()[0] == "PING")
					SEND(client.getSocket(), RPL_PONG(client.getNick(), client.getUsername(), getInput()[1]), "Error sending PONG message");
				
				else if (getInput()[0] == "PRIVMSG" && client.getStatus() == true)
					PRIVMSG(message, client);
				
				else if (getInput()[0] == "NICK" || getInput()[0] == "USER" || getInput()[0] == "PASS")
					login(client, splitMessage);
				
				else if (getInput()[0] == "MODE" && client.getStatus() == true)
					MODE(client);
				
				else if (getInput()[0] == "WHO" && client.getStatus() == true)
					WHO(fds[i].fd, client);
				
				else if (getInput()[0] == "LIST" && client.getStatus() == true)
					LIST(fds[i].fd, client);
				
				else if (getInput()[0] == "JOIN" && client.getStatus() == true)
					JOIN(fds[i].fd, client);
				
				else if (getInput()[0] == "PART" && client.getStatus() == true)
					PART(message, client);
				
				else if (splitMessage[0].find("QUIT") != std::string::npos)
					QUIT(fds, i, client);
				
				else if (getInput()[0] == "KICK" && client.getStatus() == true)
					KICK(message, client);
				
				else if (getInput()[0] == "TOPIC" && client.getStatus() == true)
					TOPIC(client);
				
				else if (getInput()[0] == "INVITE" && client.getStatus() == true)
					INVITE(client);
				
				else if ((getInput()[0] == "WHOIS" || getInput()[0] == "whois") && client.getStatus() == true)
					WHOIS(client);
				
				splitMessage.erase(splitMessage.begin());
			}
		}
	}
}

void Server::login(Client &client, std::vector<std::string> splitMessage)
{
	client.getClientLoginData(*this, splitMessage[0], getGlobalUsers(), client);
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
					it->second.setStatus(true);
			}

			if (!client.getNick().empty() && client.getNick() != it->first && !client.getUsername().empty())
			{
				this->_globalUsers.insert(std::make_pair(client.getNick(), client));
				this->_globalUsers.erase(it);
				break;
			}
		}
		it++;
	}

	if (getInput().size() == 2 && getInput()[0] == "NICK" && client.getStatus() == true)
	{
		std::map<std::string, Client> &globalUsers = getGlobalUsers();
		std::map<std::string, Client>::iterator it = globalUsers.find(client.getNick());
		if (it != globalUsers.end())
			it->second.setNick(getInput()[1]);
		
		std::map<std::string, Channel> &channels = getChannels();
		std::map<std::string, Channel>::iterator ch = channels.begin();
		while (ch != channels.end())
		{
			std::map<std::string, Client> &users = ch->second.getUsers();
			std::map<std::string, Client>::iterator us = users.find(client.getNick());
			if (us != globalUsers.end())
				us->second.setNick(getInput()[1]);

			std::vector<std::string> &operators = ch->second.getOperators();
			std::vector<std::string>::iterator op = find(operators.begin(), operators.end(), "@" + getInput()[1]);
			if (op != operators.end())
			{
				*op = "@" + getInput()[1];
			}
		}	
		
	}
}
