/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpicoli- <lpicoli-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:50:44 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/27 20:36:06 by lpicoli-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_
#define _CHANNEL_

#include "ircserv.hpp"

/**
 * @brief Represents a channel in an IRC server.
 */

class Channel {

private:
	std::string _name;                           /* Channel name */
	std::string _topic;                          /* Channel topic description */
	std::string _password;                       /* Channel password */
	bool _isInviteOnly;                          /* Flag indicating if the channel is invite-only */
	bool _isPrivate;                             /* Flag indicating if the channel is private */
	bool _restrictedTopic;                       /* Flag indicating if the topic is restricted to operators */
	int _userLimit;                              /* Maximum number of users in the channel */
	std::map<std::string, Client> _clients;      /* Map of clients in the channel, with nicknames as keys */
	std::vector<std::string> _operators;         /* List of channel operators (nicknames) */
	std::vector<std::string> _invited;           /* List of invited users (nicknames) */
	std::vector<char> _modes;                    /* List of channel modes */
	time_t _creationTime;                        /* Channel creation time */
	Client *_bot;

public:


	/**
	 * @brief Constructs a Channel object with the given name and privacy setting.
	 * 
	 * @param name The name of the channel.
	 * @param isPrivate Flag indicating if the channel is private.
	 */
	Channel(std::string name, bool isPrivate);

	/**
	 * @brief Gets the creation time of the channel.
	 * 
	 * @return The creation time of the channel as a string.
	 */
	std::string getCreationTime(void);
	Client* getBot(void);
	void setBot(Client *bot);

	bool botExists(void);

	/**
	 * @brief Gets the name of the channel.
	 * 
	 * @return The name of the channel.
	 */
	std::string getName(void);

	/**
	 * @brief Gets the topic of the channel.
	 * 
	 * @return The topic of the channel.
	 */
	std::string getTopic(void);

	/**
	 * @brief Gets the password of the channel.
	 * 
	 * @return The password of the channel.
	 */
	std::string getPassword(void);

	/**
	 * @brief Checks if the channel is invite-only.
	 * 
	 * @return True if the channel is invite-only, false otherwise.
	 */
	bool getInvisibility(void);

	/**
	 * @brief Checks if the channel has restricted access for operators.
	 * 
	 * @return True if the channel has restricted access for operators, false otherwise.
	 */
	bool getModeRestrictedAccess(void);

	/**
	 * @brief Checks if the channel has a restricted topic for operators.
	 * 
	 * @return True if the channel has a restricted topic for operators, false otherwise.
	 */
	bool getRestrictedTopic(void);

	/**
	 * @brief Gets the number of users in the channel.
	 * 
	 * @return The number of users in the channel.
	 */
	int getNbrUsers(void);

	/**
	 * @brief Gets the user limit of the channel.
	 * 
	 * @return The user limit of the channel.
	 */
	int getUserLimit(void);

	/**
	 * @brief Gets the modes of the channel.
	 * 
	 * @return The modes of the channel.
	 */
	std::vector<char> getModes(void);
	
	bool isNewMode(char mode);

	/**
	 * @brief Gets the map of users in the channel.
	 * 
	 * @return The map of users in the channel.
	 */
	std::map<std::string, Client> &getUsers(void);

	/**
	 * @brief Gets the list of operators in the channel.
	 * 
	 * @return The list of operators in the channel.
	 */
	std::vector<std::string> &getOperators(void);

	/**
	 * @brief Gets the list of invited users in the channel.
	 * 
	 * @return The list of invited users in the channel.
	 */
	std::vector<std::string> &getInvitedUsers(void);

	/**
	 * @brief Sets the name of the channel.
	 * 
	 * @param name The new name of the channel.
	 */
	void setName(std::string name);

	/**
	 * @brief Sets the topic of the channel.
	 * 
	 * @param topic The new topic of the channel.
	 */
	void setTopic(std::string topic);

	/**
	 * @brief Sets the invisibility mode of the channel.
	 * 
	 * @param mode The new invisibility mode of the channel.
	 */
	void setInvisibility(bool mode);

	/**
	 * @brief Sets the restricted access mode for operators in the channel.
	 * 
	 * @param mode The new restricted access mode for operators in the channel.
	 */
	void setModeRestrictedAccess(bool mode);

	/**
	 * @brief Sets the restricted topic mode for operators in the channel.
	 * 
	 * @param mode The new restricted topic mode for operators in the channel.
	 */
	void setRestrictedTopic(bool mode);

	/**
	 * @brief Sets the user limit of the channel.
	 * 
	 * @param limit The new user limit of the channel.
	 */
	void setUserLimit(int limit);

	/**
	 * @brief Adds a new user to the channel.
	 * 
	 * @param client The new user to add to the channel.
	 */
	void setNewUser(Client client);

	/**
	 * @brief Sets the password of the channel.
	 * 
	 * @param password The new password of the channel.
	 */
	void setPassword(std::string password);

	/**
	 * @brief Adds a new mode to the channel.
	 * 
	 * @param mode The new mode to add to the channel.
	 */
	void setNewMode(char mode);

	/**
	 * @brief Deletes a mode from the channel.
	 * 
	 * @param mode The mode to delete from the channel.
	 */
	void deleteMode(char mode);

	/**
	 * @brief Adds an operator to the channel.
	 * 
	 * @param nickname The nickname of the operator to add.
	 */
	void AddOperator(const std::string &nickname);

	/**
	 * @brief Removes an operator from the channel.
	 * 
	 * @param nickname The nickname of the operator to remove.
	 */
	void RemoveOperator(std::string nickname);

	/**
	 * @brief Adds an invited user to the channel.
	 * 
	 * @param nickname The nickname of the invited user to add.
	 */
	void AddInvited(const std::string &nickname);

	/**
	 * @brief Removes an invited user from the channel.
	 * 
	 * @param nickname The nickname of the invited user to remove.
	 */
	void RemoveInvited(std::string nickname);
};

#endif

