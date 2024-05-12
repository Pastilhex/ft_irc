/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:48:59 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
#define _SERVER_

#include "ircserv.hpp"
#include "Client.hpp"
class Channel;

class Server
{
private:
	int _socket;								/* The server socket file descriptor. */
	int _port;									/* The port number on which the server is running. */
	struct sockaddr_in _address;				/* The server address. */
	std::string _hostname;						/* The hostname of the server. */
	std::string _password;						/* The password required to connect to the server. */
	std::map<std::string, Channel> _channels;	/* The map of channels, where the key is the channel name and the value is the Channel object. */
	std::map<std::string, Client> _globalUsers; /* The map of global users, where the key is the client nickname and the value is the Client object. */
	std::vector<std::string> _input;			/* The vector of input commands received by the server. */
	pollfd serverPoll;							/* The pollfd structure used for polling server events. */
	Client _bot;								/* The bot object. */
	bool _quizMode;								/* The quiz mode flag. */
	std::vector<std::string> _botCommands;		/* The vector of bot commands. */

public:
	Server(void);
	int getSocket(void);
	std::string getPort(void);
	struct sockaddr_in getAddress(void);
	std::string getHostname(void) const;
	std::string getPassword(void);
	std::map<std::string, Channel> &getChannels(void);
	std::map<std::string, Client> &getGlobalUsers(void);
	std::vector<std::string> getBotCMD() const;
	std::string getAddressIP(void);
	std::string getClientNick(std::string &channelName, std::string &clientName);
	std::string getOpNick(std::string &channelName, std::string clientName);
	std::vector<std::string> getInput(void);
	Client &getClientBySocket(int socket, Client &client);
	bool isUserInvited(std::string user, std::string channel);
	void setSocket(int newSocket);
	void setAddress(struct sockaddr_in newAddress);
	bool setHostname(void);
	bool listenSocket(const int &serverSocket);
	void setNewChannel(std::string channel_name, Channel channel);
	void setInput(std::string input);
	void INVITE(Client client);
	void JOIN(int clientSocket, Client &client);
	void KICK(std::string message, Client client);
	void LIST(int clientSocket, Client &client);
	void MODE(Client client);
	void PART(std::string message, Client &client);
	void PRIVMSG(std::string message, Client client);
	void TOPIC(Client &client);
	void WHO(int clientSocket, const Client client);
	void WHOIS(const Client client);
	void QUIT(std::vector<pollfd> &fds, int i, const Client client);
	bool addClientToGlobalUsers(Client client);
	bool bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress);
	bool checkInput(std::vector<std::string> input, Client client);
	void connectClient(const int &serverSocket);
	bool createHostname(void);
	int createSocket(void);
	struct sockaddr_in createAddress(int port);
	void createNewClient(std::vector<pollfd> &fds, const int &serverSocket);
	void updateClient(std::vector<pollfd> &fds);
	std::vector<std::string> trimInput(std::string input);
	void removeClientFromGlobalUsers(Client client);
	bool run(void);
	void processCMD(Client &client, std::vector<pollfd> &fds, std::string message, size_t &i);
	bool start(const std::string &port, const std::string &password);
	void sendWelcome(int clientSocket, Client &client);
	void updateChannel(Channel channel);
	void broadcastKICK(Client client, std::string kicked, std::string channelName, std::string msg);
	void broadcastTOPIC(Client client, std::string channelName);
	void handlePasswordMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption);
	void handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client);
	void handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption);
	std::string handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client);
	void handleUserLimitMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client);
	void login(Client &client, std::vector<std::string> splitMessage);
	bool isDCC_SEND(std::string message, std::string sender);
	void DCC_ACCEPT(std::string sender, std::string file, std::string ip, std::string port);
	bool isValidIPAddress(std::string word);
	bool isValidPort(std::string word);
	void broadcastBot(Client client, Server server, std::string message, std::string channelName);
	bool isBotInChannel(std::string &channelName);
	bool isChannelFull(Channel &channel);
	bool canJoinChannel(const Client &client, Channel &channel);
	void joinChannel(int clientSocket, Client &client, std::string &channelName);
};

#endif