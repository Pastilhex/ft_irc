/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/06 14:04:54 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_
#define _CLIENT_

#include "ircserv.hpp"

class Client
{
private:
	std::string _nick;
	std::string _serverPassword;
	std::string _tmpPassword;
	std::string _username;
	int _socket;

public:
	/* Constructor */
	Client(void);

	/* Getters */
	std::string getNick(void) const;
	std::string getUsername(void);
	std::string getTmpPassword(void);
	int getSocket(void);

	/* Setters */
	void setNick(std::string nick);
	void setUsername(std::string username);
	void setTmpPassword(std::string pass);
	void setNewClient(Client &client);
	void setSocket(int fd);

	/* Methods */
	void getClientLoginData(char *buffer, int bytesRead);
};

#endif