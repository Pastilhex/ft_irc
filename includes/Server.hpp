/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/08 23:10:41 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
#define _SERVER_

#include "ircserv.hpp"
class Channel;
class Client;

class Server
{
private:
	int _socket;
	int _port;
	struct sockaddr_in _address;
	std::string _hostname;
	std::string _password;
	std::map<std::string, Channel> _channels; // <"Canal42":"objeto_canal"
	std::map<std::string, Client> _globalUsers;

public:
	/* Constructors*/
	Server(std::string password);

	/* Getters */
	/**
	 * @brief Obtém o descritor de arquivo do socket do servidor.
	 *
	 * Esta função retorna o descritor de arquivo do socket do servidor.
	 *
	 * @return Retorna o descritor de arquivo do socket do servidor.
	 */
	int getSocket(void);
	/**
	 * @brief Obtém a estrutura sockaddr_in do servidor.
	 *
	 * Esta função retorna a estrutura sockaddr_in que representa o endereço do servidor.
	 *
	 * @return Retorna a estrutura sockaddr_in do servidor.
	 */
	struct sockaddr_in getAddress(void);
	std::string getHostname(void) const;
	std::string getPassword(void);
	std::map<std::string, Channel> &getChannels(void);

	/* Setters */
	/**
	 * @brief Define o descritor de arquivo do socket do servidor.
	 *
	 * Esta função define o descritor de arquivo do socket do servidor para um novo valor.
	 *
	 * @param newSocket O novo valor do descritor de arquivo do socket do servidor.
	 */
	void setSocket(int newSocket);
	/**
	 * @brief Define a estrutura sockaddr_in do servidor.
	 *
	 * Esta função define a estrutura sockaddr_in que representa o endereço do servidor para um novo valor.
	 *
	 * @param newAddress O novo valor da estrutura sockaddr_in do servidor.
	 */
	void setAddress(struct sockaddr_in newAddress);
	void setHostname(std::string hostname);
	void setChannel(std::string, bool);

	/* Commands */
	void LIST(int clientSocket, Client &client, std::string message);
	void JOIN(int clientSocket, Client &client, std::string message);
	void WHO(int clientSocket, const Client client, std::string channelName);
	void PART(std::string message, Client &client);
	void MODE(std::string message, Client client);
	void PONG(std::string message, Client client);
	void KICK(std::string message, Client client);


	void Send_WHO_toAll(Client client, std::string channelName);
	void Send_PRIVMSG_toChannel(std::string message, Client client);

	/* Methods */
	/**
	 * @brief Função para obter o socket do servidor.
	 *
	 * Esta função cria e configura um socket do servidor.
	 *
	 * @return Retorna o descritor de arquivo do socket do servidor se bem-sucedido,
	 *         -1 se ocorrer um erro ao criar o socket,
	 *         1 se ocorrer um erro ao definir o modo não-bloqueante para o socket.
	 */
	int createSocket(void);

	/**
	 * @brief Cria uma estrutura sockaddr_in para o servidor.
	 *
	 * Esta função cria e configura uma estrutura sockaddr_in para representar o endereço do servidor.
	 *
	 * @param port A porta do servidor.
	 * @return Retorna a estrutura sockaddr_in configurada.
	 */
	struct sockaddr_in createAddress(int port);

	/**
	 * @brief Verifica se uma porta é válida.
	 *
	 * Esta função verifica se uma  std::string representa uma porta válida.
	 *
	 * @param str A  std::string contendo a porta a ser verificada.
	 * @return Retorna true se a porta for válida, false caso contrário.
	 */
	static bool isValidPort(char *str);
	/**
	 * @brief Inicia o servidor.
	 *
	 * Esta função inicia o servidor, configurando o socket e o endereço para comunicação.
	 *
	 * @param str Uma  std::string contendo a porta do servidor.
	 * @return Retorna true se o servidor foi iniciado com sucesso,
	 *         false caso contrário.
	 */
	bool start(char *str);
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
	bool bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress);
	bool run(void);

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
	bool checkConnections(const int &serverSocket);

	void createHostname(void);
	void connectClient(const int &serverSocket);
	void sendWelcome(int clientSocket, Client &client);
	void processMsg(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i);
	void isNewClient(std::vector<pollfd> &fds, const int &serverSocket, struct sockaddr_in &clientAddress, Client &client);

	bool addClientToGlobalUsers(Client client);
	void removeClientFromGlobalUsers(Client client);

	/**
	 * @brief Obtém informações de endereço do servidor.
	 *
	 * Esta função obtém e exibe o endereço IP local do servidor.
	 */
	std::string getAddressIP(void);
	std::string getClientNick(std::string &channelName, std::string &clientName);
	std::string getOpNick(std::string &channelName, std::string clientName);

	Client &getClientBySocket(int socket, Client &client);
};

#endif
