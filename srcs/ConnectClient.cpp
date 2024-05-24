/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/24 21:11:09 by ialves-m         ###   ########.fr       */
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
		if (server_shutdown == true)
			break;
		int activity = poll(fds.data(), fds.size(), -1);
		usleep(500000);

		if (activity == -1 && !server_shutdown)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		createNewClient(fds, serverSocket);
		updateClient(fds);
	}
	std::cout << BLUE << "Servidor encerrado." << std::endl;
	close(serverSocket);
}

void Server::createNewClient(std::vector<pollfd> &fds, const int &serverSocket)
{
	if (fds[0].revents & POLLIN)
	{
		std::cout << "Novo cliente conectado." << std::endl;
		Client *client = new Client();
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);
		client->setPoll_fd(accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize));
		if (client->getSocket() == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			delete client;
			return;
		}
		client->setPoll_events();
		client->setPoll_revents();
		fds.push_back(client->getClientPoll());
		if (!addClientToGlobalUsers(*client))
		{
			if (client->getSocket() == -1)
			{
				std::cerr << "Erro ao aceitar a conexão com o nick: " + client->getNick() + "." << std::endl;
				close(serverSocket);
				delete client;
				return;
			}
		}
		std::cout << "Users: " << getGlobalUsers().size() << std::endl;
		
		delete client;
		return;
	}
}

void Server::updateClient(std::vector<pollfd> &fds)
{
	for (size_t i = 1; i < fds.size(); ++i)
	{
		if (fds[i].revents & POLLIN)
		{
			std::map<std::string, Client> &globalUsers = getGlobalUsers();
			std::map<std::string, Client>::iterator it;
			for (it = globalUsers.begin(); it != globalUsers.end(); ++it)
			{
				if (it->second.getSocket() == fds[i].fd)
				{
					char tmp[2048] = {0};
					int bytesRead = recv(fds[i].fd, tmp, sizeof(tmp), 0);
					std::string buffer(tmp);
					size_t ptr = 0;
					ptr = buffer.find('\n');
					if (ptr == std::string::npos)
					{
						it->second.setBuffer(buffer);
						break;
					}
					else
						it->second.setBuffer(buffer);

					if (bytesRead == -1)
						std::cerr << "Erro ao receber dados do cliente." << std::endl;
					else if (bytesRead == 0)
						QUIT(fds, i, it->second);
					else
					{
						std::string inputMessage = it->second.getBuffer();
						it->second.cleanBuffer();
						processCMD(it->second, fds, inputMessage, i);
					}
					fds[i].revents = 0;
					break;
				}
			}
		}
	}
}