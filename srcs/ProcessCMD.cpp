/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/03 14:44:49 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::processCMD(Client &client, std::vector<pollfd> &fds, std::string message, size_t &i)
{
	// std::string message(buffer, bytesRead);
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
				
				splitMessage.erase(splitMessage.begin());
			}
		}
	}
}

void Server::login(Client &client, std::vector<std::string> splitMessage)
{
	client.getClientLoginData(*this, splitMessage[0], getGlobalUsers(), getHostname());
	std::map<std::string, Client> &globalUsers = getGlobalUsers();
	std::map<std::string, Client>::iterator it = globalUsers.begin();
	while (it != globalUsers.end())
	{
		if (client.getClientPoll().fd == it->second.getClientPoll().fd)
		{
			if (!client.getNick().empty() && client.getNick() != it->second.getNick())
			{
				this->_globalUsers.insert(std::make_pair(client.getNick(), client));
				this->_globalUsers.erase(it);
				break;
			}
			
			if (!client.getUsername().empty() && client.getUsername() != it->second.getUsername())
				it->second.setUsername(client.getUsername());
			
			if (!client.getRealName().empty() && client.getRealName() != it->second.getRealName())
				it->second.setRealName(client.getRealName());
			
			if (!client.getTmpPassword().empty() && client.getTmpPassword() != it->second.getTmpPassword())
				it->second.setTmpPassword(client.getTmpPassword());
			
			if (client.getTmpPassword() == this->getPassword())
			{
				if (!client.getNick().empty() && !client.getUsername().empty())
					sendWelcome(client.getSocket(), client);
			}
			
			if (!client.getNick().empty() && !client.getUsername().empty() && !client.getTmpPassword().empty())
				it->second.setStatus(true);

			if (!client.getNick().empty() && !client.getUsername().empty() && !client.getTmpPassword().empty() && client.getTmpPassword() != this->getPassword())
			{
				std::cout << "<< " + splitMessage[0] << std::endl;
				SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error while login");
			}
		}
		it++;
	}
}
