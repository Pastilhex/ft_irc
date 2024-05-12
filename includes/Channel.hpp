/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:43:47 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
#define _CHANNEL_

#include "ircserv.hpp"

class Channel
{
private:
	std::string _name;						/* Channel name */
	std::string _topic;						/* Channel topic description */
	std::string _password;					/* Channel password */
	bool _isInviteOnly;						/* Flag indicating if the channel is invite-only */
	bool _isPrivate;						/* Flag indicating if the channel is private */
	bool _restrictedTopic;					/* Flag indicating if the topic is restricted to operators */
	int _userLimit;							/* Maximum number of users in the channel */
	std::map<std::string, Client> _clients; /* Map of clients in the channel, with nicknames as keys */
	std::vector<std::string> _operators;	/* List of channel operators (nicknames) */
	std::vector<std::string> _invited;		/* List of invited users (nicknames) */
	std::vector<char> _modes;				/* List of channel modes */
	time_t _creationTime;					/* Channel creation time */
	Client _bot;

public:
	Channel(std::string name, bool isPrivate);
	std::string getCreationTime(void);
	Client getBot(void);
	void setBot(Client bot);
	bool botExists(void);
	std::string getName(void);
	std::string getTopic(void);
	std::string getPassword(void);
	bool getInvisibility(void);
	bool getModeRestrictedAccess(void);
	bool getRestrictedTopic(void);
	int getNbrUsers(void);
	int getUserLimit(void);
	std::vector<char> getModes(void);
	bool isNewMode(char mode);
	std::map<std::string, Client> &getUsers(void);
	std::vector<std::string> &getOperators(void);
	std::vector<std::string> &getInvitedUsers(void);
	void setName(std::string name);
	void setTopic(std::string topic);
	void setInvisibility(bool mode);
	void setModeRestrictedAccess(bool mode);
	void setRestrictedTopic(bool mode);
	void setUserLimit(int limit);
	void setNewUser(Client client);
	void setPassword(std::string password);
	void setNewMode(char mode);
	void deleteMode(char mode);
	void AddOperator(const std::string &nickname);
	void RemoveOperator(std::string nickname);
	void AddInvited(const std::string &nickname);
	void RemoveInvited(std::string nickname);
};

#endif
