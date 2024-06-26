/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:36:38 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:51:20 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

void Server::MODE(Client client)
{
	std::vector<std::string> mode_cmd = getInput();
	if (checkInput(mode_cmd, client) || (mode_cmd.size() < 2))
		return;
	std::string channelName = mode_cmd[1];
	const std::map<std::string, Channel>::iterator &it = getChannels().find(channelName);
	if (mode_cmd.size() == 2)
	{
		std::vector<char> channelModes = it->second.getModes();
		std::string channelModesStr(channelModes.begin(), channelModes.end());
		std::string tmp = ":" + getHostname() + " 324 " + client.getNick() + " " + channelName + " :+" + channelModesStr + "\r\n";
		SEND(client.getSocket(), tmp, "Error sending MODE message");
		SEND(client.getSocket(), ":" + getHostname() + " 329 " + client.getNick() + " " + channelName + " :" + it->second.getCreationTime() + "\r\n", "Error sending MODE message");
		return;
	}

	std::vector<std::string> operators = it->second.getOperators();
	if (!Utils::isOperator(it->second, client.getNick()))
	{
		SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, channelName), "Error sending message");
		return;
	}

	std::string mode = mode_cmd[2];
	char modeFlag = mode[0];
	char modeOption = mode[1];
	std::string msg;
	if (modeOption == 'o' && mode_cmd.size() == 4)
		msg = handleOperatorMode(mode_cmd, it, modeOption, modeFlag, client);
	if (modeOption == 'i' && mode_cmd.size() == 3)
		handlePrivateAccessMode(it, modeOption, modeFlag, client);
	if (modeOption == 't' && mode_cmd.size() == 3)
		handleRestrictedTopicMode(it, modeFlag, client, modeOption);
	if (modeOption == 'k')
		handlePasswordMode(mode_cmd, it, modeFlag, client, modeOption);
	if (modeOption == 'l')
		handleUserLimitMode(mode_cmd, it, modeFlag, client);
}

std::string Server::handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client)
{
	if (!Utils::isValidUser(it->second, mode_cmd[3]))
		return (mode_cmd[3] + " " + it->first + " :User is not in the channel\r\n");

	if (modeFlag == '+')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
			return (mode_cmd[3] + " " + it->first + " :User is already an operator\r\n");

		it->second.AddOperator(it->second.getUsers().find(mode_cmd[3])->first);
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
			++us;
		}
		updateChannel(it->second);
		return (mode_cmd[3] + " " + it->first + " :User added as an operator\r\n");
	}
	else if (modeFlag == '-')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
		{
			it->second.RemoveOperator(mode_cmd[3]);
			std::map<std::string, Client> users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->second.getRealName() != "Bot")
					SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
				++us;
			}
			updateChannel(it->second);
			return (mode_cmd[3] + " " + it->first + " :User removed as an operator\r\n");
		}
		return (mode_cmd[3] + " " + it->first + " :User can't be removed of the operator's list because it's not an operator\r\n");
	}
	return NULL;
}

void Server::handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client)
{
	if (modeFlag == '+')
	{
		if (it->second.getInvisibility())
		{
			return (Utils::logMessage("Channel is already private", 0), void());
		}
		it->second.setInvisibility(true);
		it->second.setNewMode(modeOption);
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
			++us;
		}
		return (Utils::logMessage("Channel is now private", 0), void());
	}
	else if (modeFlag == '-')
	{
		if (it->second.getInvisibility())
		{
			it->second.setInvisibility(false);
			it->second.deleteMode(modeOption);
			std::map<std::string, Client> users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->second.getRealName() != "Bot")
					SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
				++us;
			}
			return (Utils::logMessage("Channel is now public", 0), void());
		}
		return (Utils::logMessage("Channel is already public", 0), void());
	}
}

void Server::handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption)
{
	(void)client;
	std::string channelName = it->second.getName();
	if (modeFlag == '+')
	{
		if (it->second.getRestrictedTopic())
		{
			return (Utils::logMessage("Channel topic is already restricted", 0), void());
		}
		it->second.setRestrictedTopic(true);
		it->second.setNewMode('t');
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), ":" + getHostname() + " MODE " + channelName + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
			++us;
		}
		return (Utils::logMessage("Channel topic is now restricted", 0), void());
	}
	else if (modeFlag == '-')
	{
		if (it->second.getRestrictedTopic())
		{
			it->second.setRestrictedTopic(false);
			it->second.deleteMode('t');
			std::map<std::string, Client> users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->second.getRealName() != "Bot")
					SEND(us->second.getSocket(), ":" + getHostname() + " MODE " + channelName + " " + modeFlag + modeOption + "\r\n", "Error sending MODE message");
				++us;
			}
			return (Utils::logMessage("Channel topic is now unrestricted", 0), void());
		}
	}
}

void Server::handlePasswordMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption)
{
	if (mode_cmd.size() >= 4 && modeFlag == '+')
	{
		if (it->second.getPassword() == mode_cmd[3])
			return (Utils::logMessage("You can't set the same password", 0), void());

		std::string password = mode_cmd[3];
		it->second.setPassword(password);
		it->second.setNewMode('k');
		it->second.setModeRestrictedAccess(true);
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + " :" + it->second.getPassword() + "\r\n", "Error sending MODE message");
			++us;
		}
		return (Utils::logMessage("New channel key is now set", 0), void());
	}
	else if (mode_cmd.size() == 3 && modeFlag == '-')
	{
		if (it->second.getPassword() == "")
			return (Utils::logMessage("Channel key is already null", 0), void());
		it->second.setPassword("");
		it->second.deleteMode('k');
		it->second.setModeRestrictedAccess(false);
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), ":" + client.getNick() + " MODE " + it->first + " " + modeFlag + modeOption + " :" + it->second.getPassword() + "\r\n", "Error sending MODE message");
			++us;
		}
		return (Utils::logMessage("Channel key is now removed", 0), void());
	}
	else
		SEND(client.getSocket(), ERR_NEEDMOREPARAMS(client, "MODE"), "Error sending MODE message");
}

void Server::handleUserLimitMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client)
{
	if (mode_cmd.size() >= 4 && modeFlag == '+')
	{
		try
		{
			double userLimit = atof(mode_cmd[3].c_str());
			int roundedLimit = static_cast<int>(userLimit);
			if (it->second.getUserLimit() == roundedLimit)
				return (Utils::logMessage("You can't set the same user limit", 0), void());
			it->second.setUserLimit(roundedLimit);
			if (it->second.isNewMode('l'))
				it->second.setNewMode('l');
			std::map<std::string, Client> users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->second.getRealName() != "Bot")
					SEND(us->second.getSocket(), MODE_CHANNELMSGWITHPARAM(client.getNick(), it->first, mode_cmd[2], mode_cmd[3]), "Error sending MODE message");
				++us;
			}
			return (Utils::logMessage("New user limit is now set", 0), void());
		}
		catch (const std::exception &e)
		{
			return (Utils::logMessage("Invalid user limit to be set", 1), void());
		}
	}
	else if (mode_cmd.size() <= 2 && modeFlag == '-')
	{
		if (it->second.getUserLimit() == UNLIMITED_USERS)
			return (Utils::logMessage("User limit is already unset", 0), void());
		it->second.setUserLimit(UNLIMITED_USERS);
		it->second.deleteMode('l');
		std::map<std::string, Client> users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getRealName() != "Bot")
				SEND(us->second.getSocket(), MODE_CHANNELMSGWITHPARAM(client.getNick(), it->first, mode_cmd[2], mode_cmd[3]), "Error sending MODE message");
			++us;
		}
		return (Utils::logMessage("User limit is now removed", 0), void());
	}
}
