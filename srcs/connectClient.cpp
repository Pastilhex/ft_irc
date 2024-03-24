/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectClient.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:41:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 18:44:17 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

bool	connectClient(int& serverSocket)
{
	while (true)
	{
		struct sockaddr_in clientAddress;
		socklen_t clientAddress_size = sizeof(clientAddress);
		int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddress_size);
		if (clientSocket == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			return false;
		}

		char buffer[1024];
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == -1)
		{
			std::cerr << "Erro ao receber dados do cliente." << std::endl;
			close(clientSocket);
			close(serverSocket);
			return false;
		}
		else if (bytesReceived == 0)
		{
			std::cout << "Conexão fechada pelo cliente." << std::endl;
			close(clientSocket);
		}
		else
		{
			std::cout << "Dados recebidos do cliente: " << std::string(buffer, bytesReceived) << std::endl;
		}
	}
}