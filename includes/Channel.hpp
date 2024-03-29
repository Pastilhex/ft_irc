/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/28 23:29:41 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
# define _CHANNEL_

#include "ircserv.hpp"

class Channel {
private:
	std::string	_name;
	std::string	_topic;
	std::string	_password;
	std::string	_userMode;
	bool	_modeTopic;
	bool	_modeAccess;
	bool	_modePassword;
	std::vector<std::pair<std::string, Client> > _clients; // <"user":"pastilhex"> | <"operator":"Jhonata">

public:
	/* Getters */
	std::string	getName(void);
	std::string	getTopic(void);
	std::string	getUserMode(void);
	bool	getModeTopic(void);
	bool	getModeAccess(void);
	bool	getModePassword(void);

	/* Setters */
	void	setName(std::string name);
	void	setTopic(std::string topic);
	void	setUserMode(std::string usermode);
	void	setModeTopic(bool mode);
	void	setModeAccess(bool mode);
	void	setModePassword(bool mode);
};

#endif