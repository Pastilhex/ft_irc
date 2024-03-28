/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 23:04:45 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
# define _CHANNEL_

#include "ircserv.hpp"

class Channel {
private:
	string	_name;
	string	_topic;
	string	_userMode;
	bool	_modeTopic;
	bool	_modeAccess;
	bool	_modePassword;
	vector<pair<string, Client>	> _clients; // <"user":"pastilhex"> | <"operator":"Jhonata">

public:
	/* Getters */
	string	getName(void);
	string	getTopic(void);
	string	getUserMode(void);
	bool	getModeTopic(void);
	bool	getModeAccess(void);
	bool	getModePassword(void);

	/* Setters */
	void	setName(string name);
	void	setTopic(string topic);
	void	setUserMode(string usermode);
	void	setModeTopic(bool mode);
	void	setModeAccess(bool mode);
	void	setModePassword(bool mode);
};

#endif