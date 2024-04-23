/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/23 12:55:53 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void Server::connectClient(const int &serverSocket)
{
	std::vector<pollfd> fds;
	this->serverPoll.fd = serverSocket;
	this->serverPoll.events = POLLIN;
	this->serverPoll.revents = 0;
	fds.push_back(this->serverPoll);
	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity == -1)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		createNewClient(fds, serverSocket);
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				Client client;
				std::map<std::string, Client>::iterator it_begin = _globalUsers.begin();
				std::map<std::string, Client>::iterator it_end = _globalUsers.end();
				for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
				{
					if (it->second.getSocket() == fds[i].fd)
						client = it->second;
				}
				if (client.getSocket() == 0)
					throw std::runtime_error("Cliente não encontrado");

				char buffer[1024];
				int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				}
				else if (bytesRead == 0)
				{
					QUIT(fds, i, client);
				}
				else
				{
					processCMD(client, fds, buffer, bytesRead, i);
				}
			}
		}
	}
	close(serverSocket);
}

void Server::createNewClient(std::vector<pollfd> &fds, const int &serverSocket)
{
	if (fds[0].revents & POLLIN)
	{
		Client client;
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);
		client.setPoll_fd(accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize));
		if (client.getSocket() == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			return;
		}
		client.setPoll_events();
		client.setPoll_revents();
		fds.push_back(client.getClientPoll());
		if (!addClientToGlobalUsers(client))
		{
			if (client.getSocket() == -1)
			{
				std::cerr << "Erro ao aceitar a conexão com o nick: " + client.getNick() + "." << std::endl;
				close(serverSocket);
				return;
			}
		}
	}
}
