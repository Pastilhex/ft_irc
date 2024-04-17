/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/15 07:45:11 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
#define _CHANNEL_

#include "ircserv.hpp"

class Channel
{
private:
	std::string _name;						// Channel name
	std::string _topic;						// Descrição do Tópico do canal
	std::string _password;					// channel password
	bool _isInviteOnly;						// Só permites entrada de users por convite
	bool _isPrivate;						// Public or Private(requires password) channel <sim/não>
	bool _restrictedTopic;					// Topic restricted to operators
	int _userLimit;							// Limit of users in the channel
	std::map<std::string, Client> _clients; // <"nick": obj > | <"Jhonata": client >
	std::vector<std::string> _operators;	// Channels' operators - based on the nickname
	std::vector<std::string> _invited;		// Channels' invited users - based on the nickname
	std::vector<char> _modes;				// Channel modes
	time_t _creationTime;

public:
	/* Constructors*/
	Channel(std::string name, bool _isPrivate);

	std::string getCreationTime(void);
	std::string getName(void);
	std::string getTopic(void);
	std::string getPassword(void);
	bool getInvisibility(void);
	// bool getModePrivateAccess(void);
	bool getModeRestrictedAccess(void);
	bool getRestrictedTopic(void);
	int getNbrUsers(void);
	int getUserLimit(void);
	std::vector<char> getModes(void);
	std::map<std::string, Client> &getUsers(void);
	std::vector<std::string> &getOperators(void);
	std::vector<std::string> &getInvitedUsers(void);

	/* Setters */
	// void setUserMode(std::string usermode);
	void setName(std::string name);
	void setTopic(std::string topic);
	void setInvisibility(bool mode);
	// void setModePrivateAccess(bool mode);
	void setModeRestrictedAccess(bool mode);
	void setRestrictedTopic(bool mode);
	void setUserLimit(int limit);
	void setNewUser(Client client);
	void setPassword(std::string password);
	
	void setNewMode(char mode);
	void deleteMode(char mode);

	void AddOperator(const std::string &);
	void RemoveOperator(std::string);
	
	void AddInvited(const std::string &);
	void RemoveInvited(std::string);};

#endif