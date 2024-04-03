/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/03 16:19:06 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
# define _SERVER_

#include "ircserv.hpp"
class Channel;
class Client;

class Server {
private:
	int	_socket;
	int	_port;
	struct sockaddr_in	_address;
	std::string _hostname;
	std::string _password;
	std::map<std::string, Channel> _channels; // <"Canal42":"objeto_canal"

public:
	/* Constructors*/
	Server(std::string password);

	/* Getters */
	int 							getSocket(void);
	struct sockaddr_in 				getAddress(void);
	std::string 					getHostname(void);
	std::string 					getPassword(void);
	std::map<std::string, Channel>&	getChannels(void);

	/* Setters */
	void 	setSocket(int newSocket);
	void 	setAddress(struct sockaddr_in newAddress);
	void 	setHostname(std::string hostname);
	void	setChannel(std::string, bool);

	/* Methods */
	void 				createHostname(void);
	int					createSocket(void);
	struct sockaddr_in 	createAddress(int port);
	static bool			isValidPort(char *str);
	std::string			getAddressIP(void);
	bool				start(char* str);
	bool				bindSocket(const int& serverSocket, const struct sockaddr_in& serverAddress);
	bool				run(void);
	bool				checkConnections(const int& serverSocket);
	void				connectClient(const int& serverSocket);
	void				sendWelcome(int clientSocket, Client& client);
	void				isNewClient(std::vector<pollfd>& fds, const int& serverSocket, struct sockaddr_in& clientAddress, Client& client);
	void				processMsg(Client& client, std::vector<pollfd>& fds, char* buffer, int bytesRead, int i);
	void				JOIN(int clientSocket, Client &client, std::string message);
    std::string			getClientNick(std::string& channelName, std::string& clientName);
    std::string			getOpNick(std::string& channelName, std::string clientName);
};


#endif