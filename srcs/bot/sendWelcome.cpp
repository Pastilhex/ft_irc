/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendWelcome.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 20:23:27 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/05/01 00:05:08 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::sendWelcome(Server server, Channel &channel, Client &client)
{
    std::cout << client.getNick() << " joined " << channel.getName() << std::endl;
    std::string welcomeMsg =+ "👋 Hey " + client.getNick() + ", welcome to "+ channel.getName() +" 🎉 I'm Marvin, ";
    welcomeMsg += "your friendly bot here to assist you on this exciting journey!";
    welcomeMsg += " 🤖💬 Feel free to explore, ask questions, and engage with the community.";
    welcomeMsg += " If you need assistance at any point, just type !help and"; 
    welcomeMsg += " I'll be here to lend a hand. We're thrilled to have you here! 🚀✨\r\n";
    SEND(client.getSocket(), ":Marvin!"+client.getUsername()+"@"+server.getHostname()+" PRIVMSG "+channel.getName()+" :" + welcomeMsg, "Error sending JOIN message to client.");
}

//:Zilio!lpicoli-@c2r4s6.42porto.com PRIVMSG #40 :dsdfdsfsd