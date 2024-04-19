/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 13:22:05 by ialves-m         ###   ########.fr       */
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
					std::cout << "Conexão fechada pelo cliente." << std::endl;
					std::map<std::string, Channel> &channels = this->getChannels();
					std::map<std::string, Channel>::iterator ch = channels.begin();

					std::map<std::string, Client> &globalUsers = this->getGlobalUsers();
					std::map<std::string, Client>::iterator gb = globalUsers.begin();
					while (gb != globalUsers.end())
					{
						if (gb->first == client.getNick())
							globalUsers.erase(gb++);
						else
							++gb;
					}

					while (ch != channels.end())
					{
						std::map<std::string, Client> &users = ch->second.getUsers();
						std::map<std::string, Client>::iterator us = users.begin();
						while (us != users.end())
						{
							if (us->first == client.getNick())
								users.erase(us++);
							else
								++us;
						}
						std::vector<std::string> &operators = ch->second.getOperators();
						std::vector<std::string>::iterator op = operators.begin();
						while (op != operators.end())
						{
							if (*op == client.getNick())
								operators.erase(op++);
							else
								++op;
						}
						std::vector<std::string> &invitedUsers = ch->second.getInvitedUsers();
						std::vector<std::string>::iterator in = invitedUsers.begin();
						while (in != invitedUsers.end())
						{
							if (*in == client.getNick())
								invitedUsers.erase(in++);
							else
								++in;
						}
						++ch;
					}
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
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
	// verifica se a ligação estabelicida através do poll() é para o server(novo client) ou para um client(client já conectado)
	if (fds[0].revents & POLLIN)
	{
		Client client;
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);

		// declaração de um novo client_fd (struct do tipo pollfd)
		client.setPoll_fd(accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize));
		if (client.getSocket() == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			return;
		}
		client.setPoll_events();
		client.setPoll_revents();

		// adicionamos o novo client_fd ao vector<pollfd>
		fds.push_back(client.getClientPoll());

		char buffer[1024];
		while (client.getNick().empty() || client.getUsername().empty() || client.getTmpPassword().empty() || client.getTmpPassword() != this->getPassword())
		{
			int bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0);
			std::string message(buffer, bytesRead);

			if (message.find("CAP LS") != std::string::npos)
				SEND(client.getSocket(), ":* CAP * LS :42Porto Ft_IRCv1.0\r\n", "Error sending CAP LS message to client");

			std::cout << "<< " + message << std::endl;
			client.getClientLoginData(buffer, bytesRead, getGlobalUsers(), getHostname());

			if (!client.getTmpPassword().empty() && client.getTmpPassword() != this->getPassword())
			{
				SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error while login");
				continue; // Skip to the next iteration of the outer loop
			}
		}

		// Adiciona o client à lista global de usuarios
		if (!addClientToGlobalUsers(client))
			if (client.getSocket() == -1)
			{
				std::cerr << "Erro ao aceitar a conexão com o nick: " + client.getNick() + "." << std::endl;
				close(serverSocket);
				return;
			}

		if (!client.getNick().empty() && !client.getUsername().empty())
			sendWelcome(client.getSocket(), client);
	}
}
