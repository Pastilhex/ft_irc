/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/27 22:32:18 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

/**
 * @brief Obtém o descritor de arquivo do socket do servidor.
 *
 * Esta função retorna o descritor de arquivo do socket do servidor.
 *
 * @return Retorna o descritor de arquivo do socket do servidor.
 */
int Server::getSocket(void)
{
	return this->_socket;
}

string Server::getHostname(void)
{
	return this->_hostname;
}

/**
 * @brief Obtém a estrutura sockaddr_in do servidor.
 *
 * Esta função retorna a estrutura sockaddr_in que representa o endereço do servidor.
 *
 * @return Retorna a estrutura sockaddr_in do servidor.
 */

struct sockaddr_in Server::getAddress(void)
{
	return this->_address; // Retorna a estrutura sockaddr_in do servidor
}

/**
 * @brief Define o descritor de arquivo do socket do servidor.
 *
 * Esta função define o descritor de arquivo do socket do servidor para um novo valor.
 *
 * @param newSocket O novo valor do descritor de arquivo do socket do servidor.
 */
void Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

void Server::setHostname(string hostname)
{
	this->_hostname = hostname;
}

/**
 * @brief Define a estrutura sockaddr_in do servidor.
 *
 * Esta função define a estrutura sockaddr_in que representa o endereço do servidor para um novo valor.
 *
 * @param newAddress O novo valor da estrutura sockaddr_in do servidor.
 */

void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

/**
 * @brief Verifica se uma porta é válida.
 *
 * Esta função verifica se uma string representa uma porta válida.
 *
 * @param str A string contendo a porta a ser verificada.
 * @return Retorna true se a porta for válida, false caso contrário.
 */

bool Server::isValidPort(char *str)
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
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		cerr << "Erro ao criar o socket." << endl;
		return -1;
	}

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Erro ao definir modo não-bloqueante para o socket do servidor." << std::endl;
		close(serverSocket);
		return -1;
	}

    int optval = 1;
    if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) == -1)
    {
		std::cerr << "Erro ao reutilizar o socket do servidor." << std::endl;
        return -1;
    }

	return serverSocket;
}

/**
 * @brief Cria uma estrutura sockaddr_in para o servidor.
 *
 * Esta função cria e configura uma estrutura sockaddr_in para representar o endereço do servidor.
 *
 * @param port A porta do servidor.
 * @return Retorna a estrutura sockaddr_in configurada.
 */
struct sockaddr_in Server::createAddress(int port)
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	return serverAddress;
}

/**
 * @brief Obtém informações de endereço do servidor.
 *
 * Esta função obtém e exibe o endereço IP local do servidor.
 */
void Server::getAddressInfo(void)
{
	char hostname[256];

	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		cerr << "Erro ao obter o nome do host." << endl;
		return;
	}

	struct hostent *host_info = gethostbyname(hostname);
	if (host_info == NULL || host_info->h_addr_list[0] == NULL)
	{
		cerr << "Erro ao obter o endereço IP." << endl;
		return;
	}
	char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));
	cout << "Endereço IP local do servidor: " << ip_address << endl;
}

/**
 * @brief Inicia o servidor.
 *
 * Esta função inicia o servidor, configurando o socket e o endereço para comunicação.
 *
 * @param str Uma string contendo a porta do servidor.
 * @return Retorna true se o servidor foi iniciado com sucesso,
 *         false caso contrário.
 */
bool Server::start(char* str)
{
	if (!isValidPort(str))
		return false;
	this->_port = atoi(str);
	this->_socket = createSocket();
	this->_address = createAddress(this->_port);
	return true;
}

/**
 * @brief Associa o socket do servidor a um endereço específico.
 *
 * Esta função associa o socket do servidor a um endereço IP e número de porta específicos.
 *
 * @param serverSocket O descritor de arquivo do socket do servidor.
 * @param serverAddress A estrutura sockaddr_in contendo o endereço IP e número de porta do servidor.
 * @return Retorna true se o socket foi associado ao endereço com sucesso,
 *         false caso contrário.
 */
bool Server::bindSocket(const int& serverSocket, const struct sockaddr_in& serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		cerr << "Erro ao associar o socket ao endereço." << endl;
		close(serverSocket);
		return false;
	}
	return true;
}

/**
 * @brief Verifica as conexões do servidor.
 *
 * Esta função configura o socket do servidor para entrar no modo de escuta
 * para aceitar conexões de entrada dos clientes.
 *
 * @param serverSocket O descritor de arquivo do socket do servidor.
 * @return Retorna true se o socket foi colocado no modo de escuta com sucesso,
 *         false caso contrário.
 */

bool Server::checkConnections(const int& serverSocket)
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

	pollfd serverPoll;
	serverPoll.fd = serverSocket;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;

	std::vector<pollfd> fds;
	fds.push_back(serverPoll);
	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity == -1)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		if (fds[0].revents & POLLIN)
		{
			int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
			if (clientSocket == -1)
			{
				cerr << "Erro ao aceitar conexão do cliente." << endl;
				close(serverSocket);
				return ;
			}
			pollfd clientPoll;
			clientPoll.fd = clientSocket;
			clientPoll.events = POLLIN;
			clientPoll.revents = 0;
			fds.push_back(clientPoll);

			sendWelcome(clientSocket);
		}
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
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
					std::string message(buffer, bytesRead);
					if (message.substr(0, 4) == "LIST")
					{
                        // ToDo: adicionar canal na lista de canais junto com o user associado
                        // ToDo: retornar :hostname JOIN #nome_canal
						//std::string channel = message.substr(4);
						// std::string channel = ":" + getHostname() + " JOIN " + "#ola" + "\r\n";
						std::string channel = ":localhost 322 pastilhex #test 2 :Canal42\r\n";
						send(fds[i].fd, channel.c_str(), channel.size(), 0);
						//std::cout << channel << endl; 
					}
					else
						std::cout << "Dados recebidos do cliente: " << std::string(buffer, bytesRead) << std::endl;
				}
			}
		}
	}
	close(serverSocket);
}

/**
 * @brief Executa o servidor.
 *
 * Esta função inicia o servidor e executa as etapas necessárias para aceitar conexões dos clientes.
 *
 * @return Retorna true se o servidor foi iniciado e as conexões foram verificadas com sucesso,
 *         false caso contrário.
 */
bool Server::run(void)
{
	if (getSocket() && bindSocket(getSocket(), getAddress()) && checkConnections(getSocket()))
	{
		getAddressInfo();
		connectToClient(getSocket());
		return true;
	}
	return false;
}
