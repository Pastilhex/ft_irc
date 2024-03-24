/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 13:41:20 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

int main(int argc, char* argv[])
{
	(void) argv;
	if (argc == 3)
	{
		if (!isValidPort(argv[1]))
			return 0;
		int port = std::atoi(argv[1]);
		int serverSocket = getServerSocket();
		struct sockaddr_in serverAddress = getServerAddress(port);
		if (serverSocket && bindSocketAddress(serverSocket, serverAddress) && listenConnections(serverSocket))
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
					return 1;
				}

				char buffer[1024];
				int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
				if (bytesReceived == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
					close(clientSocket);
					close(serverSocket);
					return 1;
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
		close(serverSocket);
	}

	return 0;
}
