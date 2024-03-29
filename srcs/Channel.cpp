/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:41:57 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 11:49:07 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"


Channel::Channel(std::string name, bool isPrivate)
{
    this->_name = name;
    this->_isPrivate = isPrivate;
}

/**
 * @brief Get the name of the channel.
 *
 * @return The name of the channel.
 */
std::string Channel::getName(void)
{
    return this->_name;
}

/**
 * @brief Get the topic of the channel.
 *
 * @return The topic of the channel.
 */
std::string Channel::getTopic(void)
{
    return this->_topic;
}

/**
 * @brief Get the password of the channel.
 *
 * @return The password of the channel.
 */
std::string Channel::getPassword(void)
{
    return this->_password;
}

/**
 * @brief Get the user mode of the channel.
 *
 * @return The user mode (user/operator) of the channel.
 */
std::string Channel::getUserMode(void)
{
    return this->_userMode;
}

/**
 * @brief Get the mode status of the channel's topic.
 *
 * @return True if the topic can be edited, false if topic is locked.
 */
bool Channel::getModeTopic(void)
{
    return this->_modeTopic;
}

/**
 * @brief Get the mode status of private access to the channel.
 *
 * @return True if channel is in private access mode, false if channel is public.
 */
bool Channel::getModePrivateAccess(void)
{
    return this->_isPrivate;
}

/**
 * @brief Set the name of the channel.
 *
 * @param name The name to set for the channel.
 */
void Channel::setName(std::string name)
{
    this->_name = name;
}

/**
 * @brief Set the topic of the channel.
 *
 * @param topic The topic to set for the channel.
 */
void Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

/**
 * @brief Set the user mode of the channel.
 *
 * @param usermode The user mode (user/operator) to set for the channel.
 */
void Channel::setUserMode(std::string usermode)
{
    this->_userMode = usermode;
}

/**
 * @brief Set the mode status of the channel's topic.
 *
 * @param mode Set True to allow topic editing, False to block topic editing.
 */
void Channel::setModeTopic(bool mode)
{
    this->_modeTopic = mode;
}

/**
 * @brief Set the mode status of private access to the channel.
 *
 * @param mode The mode status to set for private access. 
 * Set True to active private access only, false to active public access.
 */
void Channel::setModePrivateAccess(bool mode)
{
    this->_isPrivate = mode;	
}
