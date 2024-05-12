/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:44:57 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_
#define _CLIENT_

#include "ircserv.hpp"
class Server;

class Client
{
protected:
	std::string _nick;			 	/* The nickname of the client. */
	std::string _serverPassword; 	/* The password for connecting to the server. */
	std::string _tmpPassword;	 	/* Temporary password for authentication. */
	std::string _username;		 	/* The username of the client. */
	std::string _realname;		 	/* The real name of the client. */
	pollfd _clientPoll;			 	/* The pollfd structure for polling events on the client's socket. */
	bool _status;				 	/* Active: Has Nickname and Username. Inactive: No Nickname and no Username, only connected to server */
	std::string _buffer;			/* Individual Buffer for each client */

public:
	Client(void);
	virtual ~Client(void);
	std::string getBuffer(void);
	void setBuffer(std::string);
	void cleanBuffer(void);
	bool getStatus(void);
	void setStatus(bool);
	std::string getRealName(void) const;
	std::string getNick(void) const;
	std::string getUsername(void) const;
	std::string getTmpPassword(void);
	int getSocket(void) const;
	pollfd getClientPoll(void);
	void setNick(std::string nick);
	void setRealName(std::string realname);
	void setUsername(std::string username);
	void setTmpPassword(std::string pass);
	void setNewClient(Client &client);
	void setSocket(int fd);
	void setPoll_fd(int fd);
	void setPoll_events(void);
	void setPoll_revents(void);
	void getClientLoginData(Server server, std::map<std::string, Client> globalUsers);
	virtual void help() {}
};

#endif