/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/06 22:14:41 by lpicoli-         ###   ########.fr       */
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

	int bytesTotal = 0;
	while (true)
	{
		if (server_shutdown == true)
			break;
		char tmp[2048] = {};
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity == -1 && !server_shutdown)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		createNewClient(fds, serverSocket);
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				std::map<std::string, Client>::iterator it_begin = _globalUsers.begin();
				std::map<std::string, Client>::iterator it_end = _globalUsers.end();
				for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
				{
					if (it->second.getSocket() == fds[i].fd)
					{
						Client &client = it->second;
						if (client.getSocket() == 0)
							throw std::runtime_error("Cliente não encontrado");						

						int bytesRead = recv(fds[i].fd, tmp, sizeof(tmp), 0);
						bytesTotal += bytesRead;
						char *ptr = strchr(tmp, '\n');
						if (ptr == NULL && bytesRead != 0)
						{
							char *buf = client.getBuffer(); 
							if (strlen(buf))
							{
								char buffer[2048] = {};
								memcpy(buffer, client.getBuffer(), bytesTotal - bytesRead);
								strcat(buffer, tmp);
								client.setBuffer(buffer);
							}							
							else
								client.setBuffer(tmp);
							break;
						}
						else
						{
							char buffer[2048] = {};
							memcpy(buffer, client.getBuffer(), bytesTotal - bytesRead);
							strcat(buffer, tmp);
							client.setBuffer(buffer);
						}

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
							std::string message(client.getBuffer(), bytesTotal);
							processCMD(client, fds, message, i);
							bytesTotal = 0;
						}
						client.setBuffer(NULL);
						break;
					}
				}
				fds[i].revents = 0;
			}
		}
	}
	std::cout << BLUE << "Servidor encerrado." << std::endl;
	close(serverSocket);
}

void Server::createNewClient(std::vector<pollfd> &fds, const int &serverSocket)
{
	if (fds[0].revents & POLLIN)
	{
		Client *client = new Client();
		client->setStatus(false);
		char tmp[2048] = {};
		client->setBuffer(tmp);
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
		delete client;
		return;
	}
	return;
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
