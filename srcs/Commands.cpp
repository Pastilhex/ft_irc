#include "../includes/ircserv.hpp"

void Server::MODE(std::string message, Client client)
{
	(void)client;
	std::vector<std::string> mode_cmd = Utils::split(message, " \n\t\r");
	if (mode_cmd.size() < 2)
	{
		return;
	}
	std::string channel_name = mode_cmd[1].substr(1);
#include <iostream> // Add missing include statement

	std::map<std::string, Channel>::iterator it = getChannels().find(channel_name);
	if (it == getChannels().end())
	{
		std::string msg = client.getNick() + " " + channel_name + " :No such channel\r\n";
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
		return;
	}

	if (mode_cmd.size() == 2)
	{
		std::vector<char> channelModes = it->second.getModes();
		std::string channelModesStr(channelModes.begin(), channelModes.end());
		std::cout << "channels mode::" + channelModesStr << std::endl;
		// std::string msg = "MODE " + channel_name + " " + channelModesStr + "\r\n";
		// send(client.getSocket(), msg.c_str(), msg.size(), 0);
		// msg += "MODE " + channel_name + " " + channelModes + "\r\n";
		// send(client.getSocket(), msg.c_str(), msg.size(), 0);
		return;
	}

	std::vector<std::string> operators = it->second.getOperators();

	if (!Utils::isValidUser(it->second, client.getNick()) && Utils::isOperator(it->second, client.getNick()))
	{
		std::string msg = client.getNick() + " " + channel_name + " :You're not channel operator\r\n";
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
		return;
	}

	std::string mode = mode_cmd[2];
	char modeFlag = mode[0];
	char modeOption = mode[1];
	std::string msg;
	int clientSocket = client.getSocket();

	if (modeOption == 'o')
	{
		msg = handleOperatorMode(mode_cmd, it, modeFlag);
	}
	else if (modeOption == 'i')
	{
		handlePrivateAccessMode(it, modeOption, modeFlag);
	}
	else if (modeOption == 't')
	{
		handleRestrictedTopicMode(it, modeFlag);
	}
	else if (modeOption == 'k')
	{
		handlePasswordMode(mode_cmd, it, modeFlag);
	}
	else if (modeOption == 'l')
	{
		handleUserLimitMode(mode_cmd, it, modeFlag);
	}
	else
		msg = client.getNick() + "  :Unknown MODE flag\r\n";
	std::cout << msg << std::endl;
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

std::string Server::handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{

	if (mode_cmd.size() != 4)
	{
		return (NULL);
	}
	else if (!Utils::isValidUser(it->second, mode_cmd[3]))
	{
		return (mode_cmd[3] + " " + it->first + " :User is not in the channel\r\n");
	}

	if (modeFlag == '+')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
		{
			// TO DO: operators esta guardando o user com @ junto, assim temos problemas na verificacao 
			return (mode_cmd[3] + " " + it->first + " :User is already an operator\r\n");
		}
		it->second.AddOperator(it->second.getUsers().find(mode_cmd[3])->first);
		return (mode_cmd[3] + " " + it->first + " :User added as an operator\r\n");
	}
	else if (modeFlag == '-')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
		{
			it->second.RemoveOperator(mode_cmd[3]);
			// Zilio removes channel operator status from Zilio
			return (mode_cmd[3] + " " + it->first + " :User removed as an operator\r\n");
		}
		return (mode_cmd[3] + " " + it->first + " :User can't be removed of the operator's list because it's not an operator\r\n");
	}
	return NULL;
}

void Server::handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag)
{
	(void)modeFlag;

	if(modeFlag == '+')
	{
		if (it->second.getModePrivateAccess())
		{
			return (Utils::logMessage("Channel is already private", 0), void());
		}
		it->second.setModePrivateAccess(true);
		it->second.setNewMode('i');
		//return (Utils::logMessage("Channel is now private", 0), void());
	}
	else if (modeFlag == '-')
	{
		if (it->second.getModePrivateAccess())
		{
			it->second.setModePrivateAccess(false);
			it->second.deleteMode(modeOption);
		}
		return (Utils::logMessage("Channel is already public", 0), void());
	}
}

void Server::handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag)
{
	(void)modeFlag;
	if (it->second.getRestrictedTopic())
	{
		return (Utils::logMessage("Channel topic is already restricted", 0), void());
	}
	it->second.setRestrictedTopic(true);
	it->second.setNewMode('t');
	return (Utils::logMessage("Channel topic is now restricted", 0), void());
}

void Server::handlePasswordMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{
	(void)modeFlag;
	if (mode_cmd.size() >= 4)
	{
		std::string password = mode_cmd[3];
		it->second.setPassword(password);
		it->second.setNewMode('k');
		return (Utils::logMessage("Channel key is now set", 0), void());
	}
}

void Server::handleUserLimitMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{
	(void)modeFlag;
	if (mode_cmd.size() >= 4)
	{
		try
		{
			double userLimit = atof(mode_cmd[3].c_str());
			int roundedLimit = static_cast<int>(userLimit);
			it->second.setUserLimit(roundedLimit);
			it->second.setNewMode('l');
			return (Utils::logMessage("User limit is now set", 0), void());
		}
		catch (const std::exception &e)
		{
			return (Utils::logMessage("Removing user limit", 1), void());
			it->second.setUserLimit(UNLIMITED_USERS);
		}
	}
}
