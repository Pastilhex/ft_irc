/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:51:44 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/05/01 18:45:29 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::remove(Server &server, Channel &channel, Client &client)
{
	std::map<std::string, Client>::iterator bot;
	bot = channel.getUsers().find(channel.getBot()->getNick());
	channel.getUsers().erase(bot);
	std::string message = "🤖 Beep boop! " + client.getNick() + " thinks he/she can get rid of me, huh? Well, I'll be back! ";
	message += "Watch your back, " + client.getNick() + "! 😜\r\n";
	server.broadcastBot(client, server, message, channel.getName());
}