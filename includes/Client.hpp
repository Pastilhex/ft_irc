/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 13:33:27 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_
# define _CLIENT_

#include "ircserv.hpp"

class Client {
private:
	std::string	_nick;
	std::string	_serverPassword;
	std::string	_tmpPassword;
	std::string	_username;

public:
	/* Getters */
	std::string getNick(void);
	std::string getUsername(void);
	std::string getTmpPassword(void);
	
	/* Setters */
	void setNick(std::string nick);
	void setUsername(std::string username);
	void setTmpPassword(std::string pass);

	/* Methods */
	void	getClientLoginData(char* buffer, int bytesRead);

};

#endif