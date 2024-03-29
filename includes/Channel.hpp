/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 11:49:26 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
# define _CHANNEL_

#include "ircserv.hpp"

class Channel {
private:
	std::string	_name;			// Channel name
	std::string	_topic;			// Descrição do Tópico do canal
	std::string	_password;		// channel password
	std::string	_userMode;		// user ou operator +o/-o
	bool	_modeTopic;			// Premissão para mudar tópico <sim/não>
	bool	_isPrivate; // Public or Private channel <sim/não>
	std::vector<std::pair<std::string, Client> > _clients; // <"user":"pastilhex"> | <"operator":"Jhonata">

public:

	/* Constructors*/
	Channel(std::string name, bool _isPrivate);
	
	/* Getters */
	std::string	getName(void);
	std::string	getTopic(void);
	std::string	getPassword(void);
	std::string	getUserMode(void);
	bool	getModeTopic(void);
	bool	getModePrivateAccess(void);

	/* Setters */
	void	setName(std::string name);
	void	setTopic(std::string topic);
	void	setUserMode(std::string usermode);
	void	setModeTopic(bool mode);
	void	setModePrivateAccess(bool mode);
};

#endif