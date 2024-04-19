/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessCMD.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 22:13:27 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::processCMD(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i)
{
	std::string message(buffer, bytesRead);
	
	std::vector<std::string> splitMessage = Utils::splitVector(message, "\n");

	while (splitMessage.size())
	{

		if (message.empty() || message.compare("\n") == 0)
			return;
		else
			setInput(message);
		std::cout << RED << "<< " << RED + message;

		if (message.find("CAP END") != std::string::npos)
			SEND(fds[i].fd, ":* CAP * END\r\n", "Error sending CAP LS message to client");

		else if (isCMD(message, "PING"))
		{
			SEND(client.getSocket(), RPL_PONG(client.getNick(), client.getUsername(), getInput()[1]), "Error sending PONG message");
		}

		else if (isCMD(message, "PRIVMSG"))
		{
			PRIVMSG(message, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "NICK") || isCMD(message, "USER ") || isCMD(message, "PASS"))
		{
			client.getClientLoginData(buffer, bytesRead, getGlobalUsers(), getHostname());
			WHO(client.getSocket(), client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "MODE"))
		{
			MODE(client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "WHO"))
		{
			WHO(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "LIST"))
		{
			LIST(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "JOIN"))
		{
			JOIN(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "PART"))
		{
			PART(message, client);
			fds[i].revents = 0;
		}
		else if (message.find("QUIT") != std::string::npos)
		{

		}
		else if (isCMD(message, "KICK"))
		{
			KICK(message, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "TOPIC"))
		{
			TOPIC(client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "INVITE"))
		{
			INVITE(client);
			fds[i].revents = 0;
		}
		splitMessage.erase(splitMessage.begin());
	}
}
