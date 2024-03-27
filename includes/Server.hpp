/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 23:04:23 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_
# define _SERVER_

#include "ircserv.hpp"
using namespace std;

class Server {
private:
	int	_socket;
	int	_port;
	struct sockaddr_in	_address;
	string _hostname;
	

public:
	/* Getters */
	int getSocket(void);
	struct sockaddr_in getAddress(void);
	string getHostname(void);

	/* Setters */
	void setSocket(int newSocket);
	void setAddress(struct sockaddr_in newAddress);
	void setHostname(string hostname);

	/* Methods */
	static bool			isValidPort(char *str);
	int					createSocket(void);
	struct sockaddr_in 	createAddress(int port);
	void				getAddressInfo(void);
	bool				start(char* str);
	bool				bindSocket(const int& serverSocket, const struct sockaddr_in& serverAddress);
	bool				run(void);
	bool				checkConnections(const int& serverSocket);
	void				connectToClient(const int& serverSocket);

};

#endif