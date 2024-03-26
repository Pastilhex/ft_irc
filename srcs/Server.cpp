/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 23:06:12 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

int	Server::getSocket(void)
{
	return this->_socket;
}
struct sockaddr_in Server::getAddress(void)
{
	return this->_address;
}

void Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

bool	Server::isValidPort(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}

/**
 * @brief Função para obter o socket do servidor.
 *
 * Esta função cria e configura um socket do servidor.
 *
 * @return Retorna o descritor de arquivo do socket do servidor se bem-sucedido,
 *         -1 se ocorrer um erro ao criar o socket,
 *         1 se ocorrer um erro ao definir o modo não-bloqueante para o socket.
 */
int Server::createSocket(void)
{
    // Cria um socket TCP IPv4
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Verifica se ocorreu um erro ao criar o socket
    if (serverSocket == -1)
    {
        cerr << "Erro ao criar o socket." << endl;
        return -1; // Retorna -1 em caso de erro
    }

    // Define o socket para modo não-bloqueante
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Erro ao definir modo não-bloqueante para o socket do servidor." << std::endl;
        close(serverSocket);
        return 1; // Retorna 1 em caso de erro ao definir o modo não-bloqueante
    }

    return serverSocket; // Retorna o descritor de arquivo do socket do servidor
}

struct sockaddr_in Server::createAddress(int port)
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	return serverAddress;
}

void	Server::getAddressInfo(void)
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		cerr << "Erro ao obter o nome do host." << endl;
		return ;
	}

	struct hostent *host_info = gethostbyname(hostname);
	if (host_info == NULL || host_info->h_addr_list[0] == NULL)
	{
		cerr << "Erro ao obter o endereço IP." << endl;
		return ;
	}

	char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));
	cout << "Endereço IP local do servidor: " << ip_address << endl;
}

bool Server::start(char* str)
{
	if (!isValidPort(str))
			return false;
	this->_port = atoi(str);
	this->_socket = createSocket();
	this->_address = createAddress(this->_port);
	return true;
}

bool	Server::bindSocket(const int& serverSocket, const struct sockaddr_in& serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		cerr << "Erro ao associar o socket ao endereço." << endl;
		close(serverSocket);
		return false;
	}
	return true;
}

bool	Server::checkConnections(const int& serverSocket)
{
	if (listen(serverSocket, 5) == -1)
	{
		cerr << "Erro ao colocar o socket em modo de escuta." << endl;
		close(serverSocket);
		return false;
	}
	return true;
}

void	Server::connectToClient(const int& serverSocket)
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
        if (activity == -1)
		{
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
                    // Processar mensagem do cliente
                    std::string message(buffer, bytesRead);
                    if (message.substr(0, 4) == "JOIN")
					{
                        // Confirmar que o cliente entrou no canal
                        std::string channel = message.substr(4);
                        send(fds[i].fd, channel.c_str(), channel.size(), 0);
                    }
					else
						std::cout << "Dados recebidos do cliente: " << std::string(buffer, bytesRead) << std::endl;
				}
			}
		}
	}
	close(serverSocket);
}

bool	Server::run(void)
{
	if (getSocket() && bindSocket(getSocket(), getAddress()) && checkConnections(getSocket()))
	{
		getAddressInfo();
		connectToClient(getSocket());
		return true;
	}
	else
		return false;
}