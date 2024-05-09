/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/08 20:43:46 by ialves-m         ###   ########.fr       */
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
	//client.cleanBuffer();
}
