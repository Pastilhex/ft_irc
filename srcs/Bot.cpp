/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:04:11 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/25 15:39:10 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

#define BOT_NICKNAME this->getNick()

Bot::Bot(std::string nick, Server &server) : Client() // Constructor
{
	if (nick.empty())
		this->setNick("Bot");
	else
		this->setNick(nick);
	this->server = server;
	this->_username = "Marvin";
	this->_realname = "Bot";
}

// Client Bot::createBot(Server *server) // Create a bot
// {
// 	Client *bot = new Bot("Bot", *server);
// 	bot->setUsername("Marvin");
// 	return *bot;
// }

void Bot::welcomeNewClient(Client client) // Welcome a new client
{

	std::cout << "Welcome to the server!" << client.getNick() << std::endl;

	// std::string msg = ":" + BOT_NICKNAME + "!" + BOT_NICKNAME + "@" + server.getHostname() + " PRIVMSG " + BOT_NICKNAME + " :" + "Welcome to the server, " + client.getNick() + "!\r\n";
	// send(server.getSocket(), msg.c_str(), msg.length(), 0);
}

bool Server::BOT(std::vector<std::string> &input) // Check if there is already a bot in the channel
{
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName = "";
	std::string msgToSend = getMessage(input[2]);

	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			break;
		}
		++inputIterator;
	}

	std::map<std::string, Channel>::iterator it = this->getChannels().find(channelName);
	if (it == this->getChannels().end())
		return false;

	std::map<std::string, Client> &users = it->second.getUsers();
	std::map<std::string, Client>::iterator us = users.begin();
	while (us != users.end())
	{
		if (us->second.getRealName() == "Bot")
			return false;
		us++;
	}

	return true;
}

/* Client Bot::createBot(Server *server)
{
	Client* bot = new Bot("Bot", *server);
	return *bot;
}
 */