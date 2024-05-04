/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/03 15:02:42 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

// volatile sig_atomic_t server = 0;

// void signalHandler(int signum)
// {
// 	std::cout << RED << "Signal (" << signum << ") received." << std::endl;
// 	std::cout << RESET << std::endl;
// 	if (signum == SIGINT)
// 		server = 1;
// 	else if (signum == SIGTSTP)
// 		server = 2;
// }

void Server::connectClient(const int &serverSocket)
{
	std::vector<pollfd> fds;
	this->serverPoll.fd = serverSocket;
	this->serverPoll.events = POLLIN;
	this->serverPoll.revents = 0;
	fds.push_back(this->serverPoll);

	// signal(SIGINT, signalHandler);
	// signal(SIGTSTP, signalHandler);

	// char buffer[1024] = {};
	int bytesTotal = 0;
	while (true)
	{
		char tmp[2048] = {};
		// if (server == 1)
		// 	break;
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

				
				int bytesRead = recv(fds[i].fd, tmp, sizeof(client.getBuffer()), 0); // <<------------ RECEIVES DATA <<---------------- //
				client.setBuffer(tmp);
				// memcpy(buffer, tmp, bytesRead);
				bytesTotal += bytesRead;
				char *ptr = strchr(tmp, '\n');
				if (ptr == NULL && bytesRead != 0)
					break;

				if (bytesRead == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				}
				else if (bytesRead == 0)// || server == 2
				{
					QUIT(fds, i, client);
				}
				else
				{
					std::string message(client.getBuffer(), bytesRead);
					processCMD(client, fds, message, i);
					// memset(buffer, '\0', sizeof(buffer));
					// memset(tmp, '\0', sizeof(tmp));
					bytesTotal = 0;
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
