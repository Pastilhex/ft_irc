/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOIS.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/06 12:01:00 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::WHOIS(const Client client)
{
	// :harpy.de.SpotChat.org 311 ivo systwi ~systwi SpotChat-na4j3d.res.spectrum.com * :systwi
	// :harpy.de.SpotChat.org 319 ivo systwi :@#test #linux #spotchat #linuxmint-nl #linuxmint-chat #linuxmint-help
	// :harpy.de.SpotChat.org 312 ivo systwi harpy.de.SpotChat.org :is it a Bird?
	// :harpy.de.SpotChat.org 330 ivo systwi systwi :is logged in as
	// :harpy.de.SpotChat.org 671 ivo systwi :is using a secure connection
	// :harpy.de.SpotChat.org 317 ivo systwi 194282 1714798951 :seconds idle, signon time
	// :harpy.de.SpotChat.org 318 ivo systwi :End of /WHOIS list.

	std::map<std::string, Client> &globalUsers = getGlobalUsers();
	std::map<std::string, Client>::iterator it = globalUsers.find(getInput()[1]);
	if (it != globalUsers.end())
	{
		SEND(client.getSocket(), RPL_WHOISUSER(client, it->second), "Error sending WHOIS message");
	}
}
