/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendGoodbye.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 20:23:27 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/04/30 22:30:38 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::sendGoodbye(Server server, Channel &channel, Client &client)
{
    std::cout << client.getNick() << " left " << channel.getName() << std::endl;
    std::string goodbyeMsg =+ "👋 Goodbye " + client.getNick() + "! We'll miss you here at "+ channel.getName() +". ";
    goodbyeMsg += "If you ever want to come back, know that you're always welcome! 🌟";
    goodbyeMsg += " Until then, take care and see you soon! 👋😢\r\n";
	server.broadcastBot(client, server, goodbyeMsg, channel.getName());
	server.updateChannel(channel);
    //SEND(client.getSocket(), ":Marvin!"+client.getUsername()+"@"+server.getHostname()+" PRIVMSG "+channel.getName()+" :" + goodbyeMsg, "Error sending PART message to client.");
}


//:Zilio!lpicoli-@c2r4s6.42porto.com PRIVMSG #40 :dsdfdsfsd