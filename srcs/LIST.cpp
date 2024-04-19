/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 12:34:10 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::LIST(int clientSocket, Client &client)
{
	std::map<std::string, Channel> channels = this->getChannels();
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		size_t pos = channelName.find('\n');
		if (pos != std::string::npos)
		{
			channelName.erase(pos, 1);
		}
		int nbrUser = it->second.getNbrUsers();
		char nbrUserStr[20];				// Tamanho suficiente para armazenar um inteiro
		sprintf(nbrUserStr, "%d", nbrUser); // Formatar o inteiro como uma string
		std::string channel = RPL_LIST(client, nbrUserStr, it);
		channel += RPL_LISTEND(client);
		SEND(clientSocket, channel, "Error updating LIST");
	}
}