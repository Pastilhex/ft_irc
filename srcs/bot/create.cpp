/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:50:15 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/04/30 22:29:31 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::create(Server &server, Channel &channel, std::string bot_nick, Client &client)
{
	Client *bot = new Client();
	if (bot_nick.empty())
	{
		bot->setNick("Bot");
		bot->setUsername("Bot");
	}
	else
	{
		bot->setNick(bot_nick);
		bot->setUsername(bot_nick);
	}
	bot->setRealName("Bot");
	channel.setNewUser(*bot);
	channel.setBot(bot);
	std::string message = "🤖 Beep boop! I'm alive! Hi everyone, I'm " + bot->getNick() + ", your new bot friend.\r\n";
	server.broadcastBot(client, server, message, channel.getName());
	server.updateChannel(channel);
}