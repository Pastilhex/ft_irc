/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 00:11:10 by ialves-m         ###   ########.fr       */
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
	/* Getters */
	int getSocket(void);
	struct sockaddr_in getAddress(void);
	std::string getHostname(void);
	std::string getPassword(void);

	/* Setters */
	void setSocket(int newSocket);
	void setAddress(struct sockaddr_in newAddress);
	void setHostname(std::string hostname);

	/* Methods */
	void 				createHostname(void);
	int					createSocket(void);
	struct sockaddr_in 	createAddress(int port);
	static bool			isValidPort(char *str);
	std::string				getAddressInfo(void);
	bool				start(char* str);
	bool				bindSocket(const int& serverSocket, const struct sockaddr_in& serverAddress);
	bool				run(void);
	bool				checkConnections(const int& serverSocket);
	void				connectToClient(const int& serverSocket);
	void				sendWelcome(int clientSocket, Client& client);

};


#endif