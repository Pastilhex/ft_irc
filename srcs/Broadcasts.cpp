/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Broadcasts.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 13:29:18 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::sendWelcome(int clientSocket, Client &client)
{
	std::string welcome;
	welcome += RPL_WELCOME();
	welcome += ":" + getHostname() + " 002 " + client.getNick() + " :Your host is " + getHostname() + ", running version FT_IRC_42Porto_v1.0\r\n";
	welcome += ":" + getHostname() + " 003 " + client.getNick() + " :This server was created " + getCurrentDateTime() + "\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :███████╗████████╗    ██╗██████╗  ██████╗\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██╔════╝╚══██╔══╝    ██║██╔══██╗██╔════╝\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :█████╗     ██║       ██║██████╔╝██║     \r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██╔══╝     ██║       ██║██╔══██╗██║     \r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██║        ██║ ████╗ ██║██║  ██║╚██████╗\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :╚═╝        ╚═╝  ╚══╝ ╚═╝╚═╝  ╚═╝ ╚═════╝\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :Project by:  ialves-m  lpicoli  jhogonca\r\n";
	welcome += ":" + getHostname() + " 376 " + client.getNick() + " :End of /MOTD command.\r\n";
	if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
	{
		std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
	}
}

void Server::updateChannel(Client client, std::string channelName)
{
	(void)client;
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			WHO(user_it->second.getSocket(), user_it->second);
			++user_it;
		}
		return;
	}
}

void Server::broadcastTOPIC(Client client, std::string channelName)
{
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			SEND(user_it->second.getSocket(), RPL_TOPIC(client, it->second), "Erro ao enviar mensagem de alteração de TOPIC.");
			++user_it;
		}
		return;
	}
}

void Server::broadcastKICK(Client client, std::string kicked, std::string channelName, std::string reason)
{
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			if (user_it->first != client.getNick())
				SEND(user_it->second.getSocket(), RPL_KICK(client, channelName, kicked, reason), "Error informing all users");
			++user_it;
		}
		return;
	}
}
