/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/27 12:14:26 by ialves-m         ###   ########.fr       */
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
    return this->_socket; // Retorna o descritor de arquivo do socket do servidor
}

string Server::getHostname(void)
{
    return this->_hostname; // Retorna o descritor de arquivo do socket do servidor
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
    this->_socket = newSocket; // Define o descritor de arquivo do socket do servidor
}

void Server::setHostname(string hostname)
{
    this->_hostname = hostname; // Define o descritor de arquivo do socket do servidor
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
    this->_address = newAddress; // Define a estrutura sockaddr_in do servidor
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
    // Verifica se cada caractere na string é um dígito
    for (size_t i = 0; i < strlen(str); i++)
        if (!isdigit(str[i]))
            return false; // Retorna false se encontrar um caractere que não é um dígito
    return true; // Retorna true se todos os caracteres forem dígitos
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
    // Cria uma estrutura sockaddr_in para o endereço do servidor
    struct sockaddr_in serverAddress;

    // Limpa a estrutura de endereço
    memset(&serverAddress, 0, sizeof(serverAddress));

    // Define a família de endereços para IPv4
    serverAddress.sin_family = AF_INET;

    // Define o endereço IP como INADDR_ANY, permitindo conexões em todos os interfaces de rede
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Define a porta do servidor, convertendo para o formato de rede
    serverAddress.sin_port = htons(port);

    return serverAddress; // Retorna a estrutura sockaddr_in configurada
}

/**
 * @brief Obtém informações de endereço do servidor.
 *
 * Esta função obtém e exibe o endereço IP local do servidor.
 */
void Server::getAddressInfo(void)
{
    char hostname[256];
    // Obtém o nome do host local
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        cerr << "Erro ao obter o nome do host." << endl;
        return; // Retorna se ocorrer um erro ao obter o nome do host
    }

    // Obtém as informações do host usando o nome do host
    struct hostent *host_info = gethostbyname(hostname);
    if (host_info == NULL || host_info->h_addr_list[0] == NULL)
    {
        cerr << "Erro ao obter o endereço IP." << endl;
        return; // Retorna se ocorrer um erro ao obter o endereço IP
    }

    // Converte o endereço IP para uma representação de string
    char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));

    // Exibe o endereço IP local do servidor
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
    // Verifica se a porta fornecida é válida
    if (!isValidPort(str))
        return false; // Retorna false se a porta não for válida

    // Converte a porta de string para inteiro
    this->_port = atoi(str);

    // Cria o socket do servidor
    this->_socket = createSocket();

    // Cria o endereço do servidor usando a porta fornecida
    this->_address = createAddress(this->_port);

    return true; // Retorna true se o servidor foi iniciado com sucesso
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
    // Associa o socket ao endereço IP e número de porta específicos
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "Erro ao associar o socket ao endereço." << endl;
        close(serverSocket);
        return false; // Retorna false em caso de erro
    }

    return true; // Retorna true se bem-sucedido
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
    // Coloca o socket em modo de escuta
    if (listen(serverSocket, 5) == -1)
    {
        cerr << "Erro ao colocar o socket em modo de escuta." << endl;
        close(serverSocket);
        return false; // Retorna false em caso de erro
    }

    return true; // Retorna true se bem-sucedido
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
                        // Extrair o nome do canal da mensagem
                        std::string::size_type pos = message.find(" ");
                        if (pos != std::string::npos)
                        {
                            std::string channel = message.substr(pos + 1);

                            // Enviar comando JOIN para o servidor IRC
                            std::string joinMessage = ":" + this->getHostname() + " JOIN " + channel + "\r\n";
                            send(fds[i].fd, joinMessage.c_str(), joinMessage.size(), 0);
                            std::cout << joinMessage << endl;
                            
                            // Configurar modos do canal (opcional)
                            // std::string mode_cmd = "MODE " + channel + " +tns\r\n";
                            // send(fds[i].fd, mode_cmd.c_str(), mode_cmd.length(), 0);
                        }
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
    // Verifica se o socket do servidor está configurado, se está associado a um endereço válido
    // e se as conexões foram verificadas com sucesso
    if (getSocket() && bindSocket(getSocket(), getAddress()) && checkConnections(getSocket()))
    {
        // Obtém e exibe informações de endereço do servidor
        getAddressInfo();

        // Conecta-se com o cliente
        connectToClient(getSocket());

        return true; // Retorna true se todas as etapas foram concluídas com sucesso
    }
    else
    {
        return false; // Retorna false se alguma etapa falhou
    }
}
