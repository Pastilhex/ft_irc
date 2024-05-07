/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/07 21:31:16 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::LIST(int clientSocket, Client &client)
{
	std::map<std::string, Channel> channels = this->getChannels();
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		size_t pos = channelName.find('\n');

		if (pos != std::string::npos)
			channelName.erase(pos, 1);
		if (channelName[0] == '#')
		{
			int nbrUser = it->second.getNbrUsers();
			char nbrUserStr[20];

			sprintf(nbrUserStr, "%d", nbrUser);
			std::string clientStr = client.getNick();
			std::string channel = RPL_LIST(clientStr, nbrUserStr, it);

			channel += RPL_LISTEND(client);
			SEND(clientSocket, channel, "Error updating LIST");
		}
	}
}
