/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Broadcasts.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/08 20:46:22 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::sendWelcome(int clientSocket, Client &client)
{
	std::string welcome;
	welcome += RPL_WELCOME();
	welcome += RPL_YOURHOST();
	welcome += RPL_CREATED();
	welcome += RPL_MOTD(" :               _-o#&&*''''?d:>b-_ \r\n");
	welcome += RPL_MOTD(" :           _ol''''  '',, dMF9MMMMMHo_ \r\n");
	welcome += RPL_MOTD(" :        .o##'        `'MbHMMMMMMMMMMMHo. \r\n");
	welcome += RPL_MOTD(" :      .oloo'         vodM*$&&HMMMMMMMMMM?. \r\n");
	welcome += RPL_MOTD(" :     ,               $M&ood,~'`(&##MMMMMMH' \r\n");
	welcome += RPL_MOTD(" :    -               ,MMMMMMM#b?#bobMMMMHMMML \r\n");
	welcome += RPL_MOTD(" :   L              ?MMMMMMMMMMMMMMMMM7MMM$.*Hk \r\n");
	welcome += RPL_MOTD(" :  *$.            :MMMMMMMMMMMMMMMMMMM/HMMM|`*L \r\n");
	welcome += RPL_MOTD(" : |               |MMMMMMMMMMMMMMMMMMMMbMH'   T, \r\n");
	welcome += RPL_MOTD(" : LH#:███████╗████████╗MMMM██╗██████╗MM██████╗`? \r\n");
	welcome += RPL_MOTD(" : MMMH██╔════╝╚══██╔══╝*'''██║██╔══██╗██╔════╝ - \r\n");
	welcome += RPL_MOTD(" : MMMM█████╗     ██║       ██║██████╔╝██║      : \r\n");
	welcome += RPL_MOTD(" : HMMM██╔══╝     ██║       ██║██╔══██╗██║      . \r\n");
	welcome += RPL_MOTD(" : ?MMM██║HL      ██║ ████╗ ██║██║MM██║╚██████╗ - \r\n");
	welcome += RPL_MOTD(" : -?MM╚═╝MH      ╚═╝  ╚══╝ ╚═╝╚═╝MM╚═╝ ╚═════╝ ' \r\n");
	welcome += RPL_MOTD(" :  :|MMMMMM-                 `MMMMMMMT .M|.   : \r\n");
	welcome += RPL_MOTD(" :   .9MMM[                    &MMMMM*' `'    . \r\n");
	welcome += RPL_MOTD(" :    :9MMk                    `MMM#'        - \r\n");
	welcome += RPL_MOTD(" :      &Ml                     `           ; \r\n");
	welcome += RPL_MOTD(" :       `&.                              : \r\n");
	welcome += RPL_MOTD(" :         `~,   . MADE BY #42PORTO     ./ \r\n");
	welcome += RPL_MOTD(" :             . _                  _ - \r\n");
	welcome += RPL_MOTD(" :               '`--._,dd###pp=xxx'\r\n");
	welcome += RPL_MOTD(" :\r\n");
	welcome += RPL_MOTD(" :    Project by:  ialves-m  lpicoli  jhogonca\r\n");
	welcome += RPL_ENDOFMOTD();
	if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
		std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
}

void Server::updateChannel(Channel channel)
{
	std::map<std::string, Client> &users = channel.getUsers();
	std::map<std::string, Client>::iterator user_it = users.begin();
	while (user_it != users.end())
	{
		if (user_it->second.getRealName() != "Bot")
			WHO(user_it->second.getSocket(), user_it->second);
		++user_it;
	}
	return;
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
			if (user_it->second.getRealName() != "Bot")
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
			if (user_it->first != client.getNick() && user_it->second.getRealName() != "Bot")
				SEND(user_it->second.getSocket(), RPL_KICK(client, channelName, kicked, reason), "Error informing all users");
			++user_it;
		}
		return;
	}
}

void Server::broadcastBot(Client client, Server server, std::string message, std::string channelName)
{
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			if (user_it->second.getRealName() != "Bot")
				SEND(user_it->second.getSocket(), ":Marvin!" + client.getUsername() + "@" + server.getHostname() + " PRIVMSG " + channelName + " :" + message + "\r\n", "Error sending PART message to client.");
			++user_it;
		}
		return;
	}
}
