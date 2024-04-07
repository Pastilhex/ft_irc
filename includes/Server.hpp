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
	int getSocket(void);
	struct sockaddr_in getAddress(void);
	std::string getHostname(void) const;
	std::string getPassword(void);
	std::map<std::string, Channel> &getChannels(void);

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

	std::string getAddressIP(void);
	std::string getClientNick(std::string &channelName, std::string &clientName);
	std::string getOpNick(std::string &channelName, std::string clientName);

	Client &getClientBySocket(int socket, Client &client);
};

#endif
