/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_connectClient.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:41:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 15:39:04 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void	ft_connectClient(int& serverSocket)
{
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	// Inicialização da matriz de estruturas pollfd
	pollfd serverPoll;
	serverPoll.fd = serverSocket;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;

	std::vector<pollfd> fds;
	fds.push_back(serverPoll); // Adiciona o socket do servidor à matriz

	while (true)
	{
        int activity = poll(fds.data(), fds.size(), -1); // Aguarda atividade nos sockets
        if (activity == -1) {
            std::cerr << "Erro ao chamar poll()." << std::endl;
            break;
        }

        if (fds[0].revents & POLLIN) // Verifica se há atividade no socket do servidor
		{
			int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
			if (clientSocket == -1)
			{
				cerr << "Erro ao aceitar conexão do cliente." << endl;
				close(serverSocket);
				return ;
			}

			// Adicionar o novo socket à matriz de estruturas pollfd
			pollfd clientPoll;
			clientPoll.fd = clientSocket;
			clientPoll.events = POLLIN;
			clientPoll.revents = 0;
			fds.push_back(clientPoll);

			sendWelcome(clientSocket); // Envia msg de boas vindas
		}

		
		for (size_t i = 1; i < fds.size(); ++i) // Verificar se há atividade(dados) nos sockets dos clientes para ler
		{
			if (fds[i].revents & POLLIN) // Socket do cliente pronto para leitura
			{
				char buffer[1024];
				int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				}
				else if (bytesRead == 0)
				{
					std::cout << "Conexão fechada pelo cliente." << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
				{
					std::cout << "Dados recebidos do cliente: " << std::string(buffer, bytesRead) << std::endl;
				}
			}
		}
	}
	close(serverSocket);
}