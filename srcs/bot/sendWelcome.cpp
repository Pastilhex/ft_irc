/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendWelcome.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 20:23:27 by lpicoli-          #+#    #+#             */
/*   Updated: 2024/05/08 20:54:57 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Bot::sendWelcome(Server server, Channel &channel, Client &client)
{
	std::string welcomeMsg = +"👋 Hey " + client.getNick() + ", welcome to " + channel.getName() + " 🎉 I'm Marvin, ";
	welcomeMsg += "your friendly bot here to assist you on this exciting journey!";
	welcomeMsg += " 🤖💬 Feel free to explore, ask questions, and engage with the community.";
	welcomeMsg += " If you need assistance at any point, just type !help and";
	welcomeMsg += " I'll be here to lend a hand. We're thrilled to have you here! 🚀✨\r\n";
	SEND(client.getSocket(), ":Marvin!" + client.getUsername() + "@" + server.getHostname() + " PRIVMSG " + channel.getName() + " :" + welcomeMsg, "Error sending JOIN message to client.");
}