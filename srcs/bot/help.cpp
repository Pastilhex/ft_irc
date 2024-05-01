/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:04:11 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/01 14:35:52 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

#define BOT_NICKNAME this->getNick()

void Bot::help(Server server, Channel &channel, Client &client)
{
	std::vector<std::string> helpMsg;
	helpMsg.push_back("🤖 Beep boop! Available commands:\r\n");
	helpMsg.push_back("!help    - Displays all available commands.\r\n");
	helpMsg.push_back("!quote   - Returns a random quote.\r\n");
	helpMsg.push_back("!create  - Creates a bot in the current room.\r\n");
	helpMsg.push_back("!delete  - Deletes the bot in the current room.\r\n");
	helpMsg.push_back("!welcome - Displays the welcome message.\r\n");
	helpMsg.push_back("!goodbye - Displays the goodbye message.\r\n");
	std::vector<std::string>::iterator it = helpMsg.begin();
	while (it != helpMsg.end())
		SEND(client.getSocket(), ":Marvin!" + client.getUsername() + "@" + server.getHostname() + " PRIVMSG " + channel.getName() + " :" + *it++, "Error sending JOIN message to client.");
}
