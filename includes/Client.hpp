/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/16 06:57:45 by ialves-m         ###   ########.fr       */
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
	pollfd clientPoll;


public:
	/* Constructor */
	Client(void);

	/* Getters */
	std::string getNick(void) const;
	std::string getUsername(void);
	std::string getTmpPassword(void);
	int getSocket(void) const;
	pollfd getClientPoll(void);
	
	
	/* Setters */
	void setNick(std::string nick);
	void setUsername(std::string username);
	void setTmpPassword(std::string pass);
	void setNewClient(Client &client);
	void setSocket(int fd);
	void setPoll_fd(int fd);
	void setPoll_events(void);
	void setPoll_revents(void);

	/* Methods */
	void getClientLoginData(char *buffer, int bytesRead, std::map<std::string, Client> globalUsers, std::string hostname);
};

#endif