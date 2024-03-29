/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/28 23:29:15 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_
# define _CLIENT_

#include "ircserv.hpp"

class Client {
private:
	std::string	_nick;
	std::string	_serverPassword;
	std::string	_username;

public:
	/* Getters */
	std::string getNick(void);
	std::string getUsername(void);

	/* Setters */
	void setNick(std::string nick);
	void setUsername(std::string username);

	/* Methods */
	void				getClientLoginData(char* buffer, int bytesRead);

};

#endif