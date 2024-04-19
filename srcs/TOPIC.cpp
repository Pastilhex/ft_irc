/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 12:32:51 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::TOPIC(Client &client)
{
	if (!checkInput(getInput(), client))
	{
		if (getInput().size() <= 3)
		{
			if (getInput().size() == 1)
			{
				SEND(client.getSocket(), ERR_NEEDMOREPARAMS(client, "TOPIC"), "Error sending TOPIC error message");
				return;
			}
			if (getInput().size() == 2 && (getInput()[1].empty() || !(getInput()[1][0] == '#' || getInput()[1][0] == '&')))
			{
				SEND(client.getSocket(), ERR_NOSUCHNICK(client, client.getNick()), "Error sending TOPIC error message");
				return;
			}
			else if (getInput().size() == 2)
			{
				std::string channelName = getInput()[1];
				std::map<std::string, Channel> &channels = getChannels();
				std::map<std::string, Channel>::iterator it = channels.find(channelName);
				if (it != channels.end())
				{
					if (it->first == channelName)
					{
						if (it->second.getTopic().empty())
							SEND(client.getSocket(), RPL_NOTOPIC(client, it->second), "Erro ao enviar mensagem de alteração de TOPIC.");
						else
							SEND(client.getSocket(), RPL_TOPIC(client, it->second), "Error while sending TOPIC message");
					}
				}
				return;
			}
			if (getInput().size() == 3 && (getInput()[1].empty() || (getInput()[1][0] != '#' || getInput()[1][0] != '&')) && getInput()[2].empty())
				SEND(client.getSocket(), ERR_NEEDMOREPARAMS(client, "TOPIC"), "Error sending TOPIC error message");
			else
			{
				std::string channelName = getInput()[1];
				std::map<std::string, Channel> &channels = getChannels();
				std::map<std::string, Channel>::iterator it = channels.find(channelName);

				bool isUserOp = false;
				bool isRestrictedTopic = it->second.getRestrictedTopic();

				std::vector<std::string> &operators = it->second.getOperators();
				std::vector<std::string>::iterator op = operators.begin();
				while (op != operators.end())
				{
					if ("@" + client.getNick() == *op)
						isUserOp = true;
					++op;
				}
				if (it != channels.end())
				{
					if (!isRestrictedTopic || isUserOp)
					{
						if (getInput()[2] == "::")
						{
							it->second.setTopic("");
							broadcastTOPIC(client, it->first);
						}
						else
						{
							it->second.setTopic(getInput()[2]);
							broadcastTOPIC(client, it->first);
						}
					}
					else
						SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, getInput()[1]), "Error while sending TOPIC message");
				}
			}
		}
		return;
	}
}
