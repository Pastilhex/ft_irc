/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/07 09:51:50 by ialves-m         ###   ########.fr       */
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
	int getSocket(void);
	struct sockaddr_in getAddress(void);
	std::string getHostname(void) const;
	std::string getPassword(void);
	std::map<std::string, Channel> &getChannels(void);
	std::string getAddressIP(void);
	std::string getClientNick(std::string &channelName, std::string &clientName);
	std::string getOpNick(std::string &channelName, std::string clientName);
	Client &getClientBySocket(int socket, Client &client);

	/* Setters */
	void setSocket(int newSocket);
	void setAddress(struct sockaddr_in newAddress);
	void setHostname(std::string hostname);
	void setChannel(std::string, bool);

	/* Commands */
	void LIST(int clientSocket, Client &client, std::string message);
	void JOIN(int clientSocket, Client &client, std::string message);
	void WHO(int clientSocket, const Client client, std::string channelName);
	void PART(Client &client, std::string channelName);
  	void MODE(std::string message, Client client);

	void Send_WHO_toAll(Client client, std::string channelName);
	void Send_PRIVMSG_toChannel(Client client, std::string channelName);

	/* Methods */
	int createSocket(void);
	struct sockaddr_in createAddress(int port);

	static bool isValidPort(char *str);
	bool start(char *str);
	bool bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress);
	bool run(void);
	bool checkConnections(const int &serverSocket);

	void createHostname(void);
	void connectClient(const int &serverSocket);
	void sendWelcome(int clientSocket, Client &client);
	void processMsg(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i);
	void isNewClient(std::vector<pollfd> &fds, const int &serverSocket, struct sockaddr_in &clientAddress, Client &client);

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
	void	handlePrivateAccessMode(std::map<std::string, Channel>::iterator it);

	/**
	 * @brief Handles the user limit mode for a channel.
	 *
	 * It takes a mode command and an iterator pointing to the channel in a map as parameters.
	 *
	 * @param mode_cmd The mode command to handle.
	 * @param it An iterator pointing to the channel in a map.
	 */
	void	handleUserLimitMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it);
	
	
	/**
	 * Handles the password mode command.
	 *
	 * It takes a vector of strings `mode_cmd` which contains the command arguments and a map iterator `it`
	 * pointing to the channel in which the mode command is being executed.
	 *
	 * @param mode_cmd The command arguments for the password mode command.
	 * @param it       An iterator pointing to the channel in which the mode command is being executed.
	 */
	void	handlePasswordMode(const std::vector<std::string>& mode_cmd, std::map<std::string, Channel>::iterator it);

	/**
	 * Handles the restricted topic mode command.
	 *
	 * It takes a map iterator `it` pointing to the channel in which the mode command is being executed.
	 *
	 * @param it An iterator pointing to the channel in which the mode command is being executed.
	 */
	void	handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it);
};

#endif
