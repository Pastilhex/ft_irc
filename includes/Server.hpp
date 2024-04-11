/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/11 19:56:17 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
#define _SERVER_

#include "ircserv.hpp"
class Channel;
class Client;

class Server
/**
 * @brief Represents a server in an IRC network.
 */

{

private:
	int _socket; /**< The socket file descriptor for the server. */
	int _port; /**< The port number on which the server is running. */
	struct sockaddr_in _address; /**< The server's address information. */
	std::string _hostname; /**< The hostname of the server. */
	std::string _password; /**< The password required to connect to the server. */
	std::map<std::string, Channel> _channels; /**< The map of channels on the server. */
	std::map<std::string, Client> _globalUsers; /**< The map of global users on the server. */

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
	std::string getAddressIP(void);
	std::string getClientNick(std::string &channelName, std::string &clientName);
	std::string getOpNick(std::string &channelName, std::string clientName);
	Client &getClientBySocket(int socket, Client &client);

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
	void TOPIC(int clientSocket, Client &client, std::string message);
	void JOIN(int clientSocket, Client &client, std::string message);
	void WHO(int clientSocket, const Client client, std::string channelName);

	void Send_WHO_toAll(Client client, std::string channelName);
	void Send_PRIVMSG_toChannel(Client client, std::string channelName);
	void PART(std::string message, Client &client);
	void MODE(std::string message, Client client);
	void KICK(std::string message, Client client);
  
	void updateChannel(Client client, std::string channelName);
	void PRIVMSG(std::string message, Client client);

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
	std::vector<std::string> trimInput(std::string input, Client client);

	bool addClientToGlobalUsers(Client client);
	void removeClientFromGlobalUsers(Client client);

	// Handles

	std::string handleOperatorMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag);
	
	/**
	 * @brief Handles the private access mode for a channel.
	 *
	 * It takes an iterator pointing to the channel in a map and performs the necessary operations.
	 *
	 * @param it An iterator pointing to the channel in a map.
	 */
	void	handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag);

	/**
	 * @brief Handles the user limit mode for a channel.
	 *
	 * It takes a mode command and an iterator pointing to the channel in a map as parameters.
	 *
	 * @param mode_cmd The mode command to handle.
	 * @param it An iterator pointing to the channel in a map.
	 */
	void	handleUserLimitMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag);
	
	
	/**
	 * Handles the password mode command.
	 *
	 * It takes a vector of strings `mode_cmd` which contains the command arguments and a map iterator `it`
	 * pointing to the channel in which the mode command is being executed.
	 *
	 * @param mode_cmd The command arguments for the password mode command.
	 * @param it       An iterator pointing to the channel in which the mode command is being executed.
	 */
	void	handlePasswordMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag);

	/**
	 * Handles the restricted topic mode command.
	 *
	 * It takes a map iterator `it` pointing to the channel in which the mode command is being executed.
	 *
	 * @param it An iterator pointing to the channel in which the mode command is being executed.
	 */
	void	handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag);
	/**
	 * @brief Obtém informações de endereço do servidor.
	 *
	 * Esta função obtém e exibe o endereço IP local do servidor.
	 */
};

#endif
