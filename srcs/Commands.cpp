#include "../includes/ircserv.hpp"
#include <iostream> // Add missing include statement

void Server::MODE(std::string message, Client client)
{
	(void)client;
	std::vector<std::string> mode_cmd = Utils::split(message, " \n\t\r");
	
	if (mode_cmd.size() < 2)
		return;

	std::string channel_name = mode_cmd[1].substr(1);

	std::map<std::string, Channel>::iterator it = getChannels().find(channel_name);
	if (it == getChannels().end())
	{
		std::string msg = client.getNick() + " " + channel_name + " :No such channel\r\n";
		send(client.getSocket(), msg.c_str(), msg.size(), 0);
		return (Utils::logMessage("Channel not found", 0), void());
	}

	if (mode_cmd.size() == 2)
	{
		std::vector<char> channelModes = it->second.getModes();
		std::string channelModesStr(channelModes.begin(), channelModes.end());
		std::cout << "channel's mode:" + channelModesStr;
		if(channelModesStr == "")
			std::cout << " no modes set";
		std::cout << std::endl;
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

	if (modeOption == 'o' && mode_cmd.size() == 3)
	{
		msg = handleOperatorMode(mode_cmd, it, modeFlag);
	}
	else if (modeOption == 'i' && mode_cmd.size() == 3)
	{
		handlePrivateAccessMode(it, modeOption, modeFlag);
	}
	else if (modeOption == 't' && mode_cmd.size() == 3)
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
	else if(Utils::isValidFlag(modeOption)) //for the cas the flag is valid but with the wrong number of arguments
		return;
	else
		msg = client.getNick() + "  :Unknown MODE flag\r\n";
	std::cout << msg << std::endl;
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

std::string Server::handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{
	if (!Utils::isValidUser(it->second, mode_cmd[3]))
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
			// Zilio removes channel operator status from pastilhex (sintaxe assim ?)
			return (mode_cmd[3] + " " + it->first + " :User removed as an operator\r\n");
		}
		return (mode_cmd[3] + " " + it->first + " :User can't be removed of the operator's list because it's not an operator\r\n");
	}
	return NULL;
}

void Server::handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag)
{
	if(modeFlag == '+')
	{
		if (it->second.getModePrivateAccess())
		{
			return (Utils::logMessage("Channel is already private", 0), void());
		}
		it->second.setModePrivateAccess(true);
		it->second.setNewMode(modeOption);
		return (Utils::logMessage("Channel is now private", 0), void());
	}
	else if (modeFlag == '-')
	{
		if (it->second.getModePrivateAccess())
		{
			it->second.setModePrivateAccess(false);
			it->second.deleteMode(modeOption);
			return (Utils::logMessage("Channel is now public", 0), void());
		}
		return (Utils::logMessage("Channel is already public", 0), void());
	}
}

void Server::handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag)
{
	if(modeFlag == '+')
	{
		// quando o canal e criado de forma privada o topico ja e restrito? 
		if (it->second.getRestrictedTopic())
		{
			return (Utils::logMessage("Channel topic is already restricted", 0), void());
		}
		it->second.setRestrictedTopic(true);
		it->second.setNewMode('t');
		return (Utils::logMessage("Channel topic is now restricted", 0), void());
	}
	else if (modeFlag == '-')
	{
		if(it->second.getRestrictedTopic())
		{
			it->second.setRestrictedTopic(false);
			it->second.deleteMode('t');
			return (Utils::logMessage("Channel topic is now unrestricted", 0), void());
		}
	}
}

void Server::handlePasswordMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{
	if (mode_cmd.size() >= 4 && modeFlag == '+')
	{
		if(it->second.getPassword() == mode_cmd[3])
			return (Utils::logMessage("You can't set the same password", 0), void());

		std::string password = mode_cmd[3];
		it->second.setPassword(password);
		it->second.setNewMode('k');
		return (Utils::logMessage("New channel key is now set", 0), void());
	}
	else if (mode_cmd.size() <= 3 && modeFlag == '-')
	{
		if(it->second.getPassword() == "")
			return (Utils::logMessage("Channel key is already null", 0), void());
		it->second.setPassword("");
		it->second.deleteMode('k');
		return (Utils::logMessage("Channel key is now removed", 0), void());
	}
}

void Server::handleUserLimitMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag)
{
	if (mode_cmd.size() >= 4 && modeFlag == '+')
	{
		try
		{
			double userLimit = atof(mode_cmd[3].c_str());
			int roundedLimit = static_cast<int>(userLimit);
			if(it->second.getUserLimit() == roundedLimit)
				return (Utils::logMessage("You can't set the same user limit", 0), void());
			it->second.setUserLimit(roundedLimit);
			it->second.setNewMode('l');
			return (Utils::logMessage("New user limit is now set", 0), void());
		}
		catch (const std::exception &e)
		{
			return (Utils::logMessage("Invalid user limit to be set", 1), void());
		}
	}
	else if(mode_cmd.size() <= 3 && modeFlag == '-')
	{
		if(it->second.getUserLimit() == UNLIMITED_USERS)
			return (Utils::logMessage("User limit is already unset", 0), void());
		it->second.setUserLimit(UNLIMITED_USERS);
		it->second.deleteMode('l');
		return (Utils::logMessage("User limit is now removed", 0), void());
	}
}
