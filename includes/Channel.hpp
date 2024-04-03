/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/03 07:12:46 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
# define _CHANNEL_

#include "ircserv.hpp"

class Channel {
private:
	std::string	_name;								// Channel name
	std::string	_topic;								// Descrição do Tópico do canal
	std::string	_password;							// channel password
	std::string	_userMode;							// user ou operator +o/-o
	bool	_modeTopic;								// Premissão para mudar tópico <sim/não>
	bool	_isPrivate; 							// Public or Private channel <sim/não>
	std::map<std::string, Client> _clients;			// <"nick": obj > | <"Jhonata": client >
	std::vector<std::string> _operators;			// Channels' operators - based on the nickname

public:

	/* Constructors*/
	Channel(std::string name, bool _isPrivate);
	
	/* Getters */
	std::string		getName(void);
	std::string		getTopic(void);
	std::string		getPassword(void);
	std::string		getUserMode(void);
	bool			getModeTopic(void);
	bool			getModePrivateAccess(void);
	int				getNbrUsers(void);
	const std::map<std::string, Client> getUsers(void);
	std::vector<std::string> getOperators;
	

	/* Setters */
	void			setName(std::string name);
	void			setTopic(std::string topic);
	void			setUserMode(std::string usermode);
	void			setModeTopic(bool mode);
	void			setModePrivateAccess(bool mode);
	void			setNewUser(Client client);
	void			AddOperator(std::string);
	void			RemoveOperator(std::string);
};

#endif