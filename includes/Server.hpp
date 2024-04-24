/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/24 21:57:01 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
#define _SERVER_

#include "ircserv.hpp"
#include "Client.hpp"
class Channel;
//class Client;

/**
 * @brief The Server class represents an IRC server.
 *
 * This class encapsulates the functionality of an IRC server, including managing connections,
 * handling client commands, and maintaining channels and users.
 */
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
	Client _bot;

public:
	/**
	 * Constructor for the Server class.
	 *
	 * @brief Represents a server in the IRC network.
	 *
	 * This class provides functionality to create and manage an IRC server.
	 * It allows setting a password for the server.
	 */
	Server(void);

	// Getters
	/**
	 * @brief Returns the socket associated with the server.
	 *
	 * @return The socket associated with the server.
	 */
	int getSocket(void);

	/**
	 * @brief Returns the address structure associated with the server.
	 *
	 * @return The address structure associated with the server.
	 */
	struct sockaddr_in getAddress(void);

	/**
	 * @brief Sets the bot client for the server.
	 * 
	 * @param bot The bot client to set.
	 */
	void setBot(Client bot);

	/**
	 * @brief Returns the hostname of the server.
	 *
	 * @return The hostname of the server.
	 */
	std::string getHostname(void) const;

	/**
	 * @brief Returns the password required to connect to the server.
	 *
	 * @return The password required to connect to the server.
	 */
	std::string getPassword(void);

	/**
	 * @brief Returns the map of channels.
	 *
	 * @return The map of channels.
	 */
	std::map<std::string, Channel> &getChannels(void);

	/**
	 * @brief Returns the map of global users.
	 *
	 * @return The map of global users.
	 */
	std::map<std::string, Client> &getGlobalUsers(void);

	Client getBot(void);
	
	/**
	 * @brief Returns the IP address of the server.
	 *
	 * @return The IP address of the server.
	 */
	std::string getAddressIP(void);

	/**
	 * @brief Returns the nickname of a client in a specific channel.
	 *
	 * @param channelName The name of the channel.
	 * @param clientName The name of the client.
	 * @return The nickname of the client in the specified channel.
	 */
	std::string getClientNick(std::string &channelName, std::string &clientName);

	/**
	 * @brief Returns the nickname of an operator in a specific channel.
	 *
	 * @param channelName The name of the channel.
	 * @param clientName The name of the operator.
	 * @return The nickname of the operator in the specified channel.
	 */
	std::string getOpNick(std::string &channelName, std::string clientName);

	/**
	 * @brief Returns the vector of input commands received by the server.
	 *
	 * @return The vector of input commands received by the server.
	 */
	std::vector<std::string> getInput(void);

	/**
	 * @brief Returns the client object associated with a specific socket.
	 *
	 * @param socket The socket of the client.
	 * @param client The client object to be updated.
	 * @return The client object associated with the specified socket.
	 */
	Client &getClientBySocket(int socket, Client &client);

	/**
	 * @brief Checks if a user is invited to a specific channel.
	 *
	 * @param user The name of the user.
	 * @param channel The name of the channel.
	 * @return True if the user is invited to the channel, false otherwise.
	 */
	bool isUserInvited(std::string user, std::string channel);

	// Setters
	/**
	 * @brief Sets the socket associated with the server.
	 *
	 * @param newSocket The new socket value.
	 */
	void setSocket(int newSocket);

	/**
	 * @brief Sets the address structure associated with the server.
	 *
	 * @param newAddress The new address structure.
	 */
	void setAddress(struct sockaddr_in newAddress);

	/**
	 * @brief Sets the hostname of the server.
	 *
	 * @param hostname The new hostname value.
	 */
	bool setHostname(void);

	bool listenSocket(const int &serverSocket);

		/**
		 * @brief Adds a new channel to the map of channels.
		 *
		 * @param channel_name The name of the new channel.
		 * @param channel The Channel object representing the new channel.
		 */
		void setNewChannel(std::string channel_name, Channel channel);

	/**
	 * @brief Sets the vector of input commands received by the server.
	 *
	 * @param input The new vector of input commands.
	 */
	void setInput(std::string input);

	// Commands
	/**
	 * @brief Sends an INVITE command to a client.
	 *
	 * @param client The client to be invited.
	 */
	void INVITE(Client client);

	/**
	 * @brief Handles a JOIN command from a client.
	 *
	 * @param clientSocket The socket of the client.
	 * @param client The client object.
	 */
	void JOIN(int clientSocket, Client &client);

	/**
	 * @brief Handles a KICK command from a client.
	 *
	 * @param message The kick message.
	 * @param client The client object.
	 */
	void KICK(std::string message, Client client);

	/**
	 * @brief Handles a LIST command from a client.
	 *
	 * @param clientSocket The socket of the client.
	 * @param client The client object.
	 */
	void LIST(int clientSocket, Client &client);

	/**
	 * @brief Handles a MODE command from a client.
	 *
	 * @param message The mode message.
	 * @param client The client object.
	 */
	void MODE(Client client);

	/**
	 * @brief Handles a PART command from a client.
	 *
	 * @param message The part message.
	 * @param client The client object.
	 */
	void PART(std::string message, Client &client);

	/**
	 * @brief Handles a PRIVMSG command from a client.
	 *
	 * @param message The PRIVMSG message.
	 * @param client The client object.
	 */
	void PRIVMSG(std::string message, Client client);

	bool BOT(Client &client, std::vector<std::string> &input);
	/**
	 * @brief Handles a TOPIC command from a client.
	 *
	 * @param client The client object.
	 */
	void TOPIC(Client &client);

	/**
	 * @brief Handles a WHO command from a client.
	 *
	 * @param clientSocket The socket of the client.
	 * @param client The client object.
	 */
	void WHO(int clientSocket, const Client client);

	void QUIT(std::vector<pollfd> fds, int i, const Client client);

	// Utils
	/**
	 * @brief Adds a client to the map of global users.
	 *
	 * @param client The client object to be added.
	 * @return True if the client was added successfully, false otherwise.
	 */
	bool addClientToGlobalUsers(Client client);

	/**
	 * @brief Binds the server socket to a specific address.
	 *
	 * @param serverSocket The server socket.
	 * @param serverAddress The server address.
	 * @return True if the socket was bound successfully, false otherwise.
	 */
	bool bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress);

	/**
	 * @brief Checks for incoming connections on the server socket.
	 *
	 * @param serverSocket The server socket.
	 * @return True if there are incoming connections, false otherwise.
	 */
	bool checkConnections(const int &serverSocket);

	/**
	 * @brief Checks the input commands received by the server.
	 *
	 * @param input The vector of input commands.
	 * @param client The client object.
	 * @return True if the input commands are valid, false otherwise.
	 */
	bool checkInput(std::vector<std::string> input, Client client);

	/**
	 * @brief Connects a client to the server.
	 *
	 * @param serverSocket The server socket.
	 */
	void connectClient(const int &serverSocket);

	/**
	 * @brief Creates the hostname of the server.
	 */
	bool createHostname(void);

	/**
	 * @brief Creates a new socket.
	 *
	 * @return The new socket file descriptor.
	 */
	int createSocket(void);

	/**
	 * @brief Creates a new server address structure.
	 *
	 * @param port The port number.
	 * @return The new server address structure.
	 */
	struct sockaddr_in createAddress(int port);

	/**
	 * @brief Creates a new client object and adds it to the vector of file descriptors.
	 *
	 * @param fds The vector of file descriptors.
	 * @param serverSocket The server socket.
	 */
	void createNewClient(std::vector<pollfd> &fds, const int &serverSocket);

	/**
	 * @brief Trims the input command string and splits it into a vector of strings.
	 *
	 * @param input The input command string.
	 * @return The vector of trimmed input commands.
	 */
	std::vector<std::string> trimInput(std::string input);

	/**
	 * @brief Removes a client from the map of global users.
	 *
	 * @param client The client object to be removed.
	 */
	void removeClientFromGlobalUsers(Client client);

	/**
	 * @brief Runs the server.
	 *
	 * @return True if the server is running, false otherwise.
	 */
	bool run(void);

	/**
	 * @brief Processes a message received from a client.
	 *
	 * @param client The client object.
	 * @param fds The vector of file descriptors.
	 * @param buffer The message buffer.
	 * @param bytesRead The number of bytes read.
	 * @param i The index of the client in the vector of file descriptors.
	 */
	void processCMD(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i);

	/**
	 * @brief Starts the server.
	 *
	 * @param str The server command.
	 * @return True if the server started successfully, false otherwise.
	 */
	bool start(const std::string &port, const std::string &password);

	/**
	 * @brief Sends a welcome message to a client.
	 *
	 * @param clientSocket The socket of the client.
	 * @param client The client object.
	 */
	void sendWelcome(int clientSocket, Client &client);

	/**
	 * @brief Updates the channel object associated with a client.
	 *
	 * @param client The client object.
	 * @param channelName The name of the channel.
	 */
	void updateChannel(Client client, std::string channelName);

	/**
	 * @brief Broadcasts a KICK message to all clients in a channel.
	 *
	 * @param client The client object.
	 * @param kicked The name of the client being kicked.
	 * @param channelName The name of the channel.
	 * @param msg The kick message.
	 */
	void broadcastKICK(Client client, std::string kicked, std::string channelName, std::string msg);

	/**
	 * @brief Broadcasts a TOPIC message to all clients in a channel.
	 *
	 * @param client The client object.
	 * @param channelName The name of the channel.
	 */
	void broadcastTOPIC(Client client, std::string channelName);

	/*Command handlers */
	/**
	 * @brief Handles the password mode of a channel.
	 *
	 * @param mode_cmd The mode command.
	 * @param it The iterator pointing to the channel in the map of channels.
	 * @param modeFlag The mode flag.
	 * @param client The client object.
	 * @param modeOption The mode option.
	 */
	void handlePasswordMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption);

	/**
	 * @brief Handles the private access mode of a channel.
	 *
	 * @param it The iterator pointing to the channel in the map of channels.
	 * @param modeOption The mode option.
	 * @param modeFlag The mode flag.
	 * @param client The client object.
	 */
	void handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client);

	/**
	 * @brief Handles the restricted topic mode of a channel.
	 *
	 * @param it The iterator pointing to the channel in the map of channels.
	 * @param modeFlag The mode flag.
	 * @param client The client object.
	 * @param modeOption The mode option.
	 */
	void handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption);

	/**
	 * @brief Handles the operator mode of a channel.
	 *
	 * @param mode_cmd The mode command.
	 * @param it The iterator pointing to the channel in the map of channels.
	 * @param modeFlag The mode flag.
	 * @param client The client object.
	 * @return The nickname of the operator.
	 */
	std::string handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client);

	/**
	 * @brief Handles the user limit mode of a channel.
	 *
	 * @param mode_cmd The mode command.
	 * @param it The iterator pointing to the channel in the map of channels.
	 * @param modeFlag The mode flag.
	 */
	void handleUserLimitMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client);

	void login(Client &client, std::vector<std::string> splitMessage);
	bool isDCC_SEND(std::string message, std::string sender);
	void DCC_ACCEPT(std::string sender, std::string file, std::string ip, std::string port);
	bool isValidIPAddress(std::string word);
	bool isValidPort(std::string word);

};

#endif
