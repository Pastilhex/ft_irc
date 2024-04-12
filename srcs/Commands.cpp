#include "../includes/ircserv.hpp"
#include <iostream> // Add missing include statement

void Server::MODE(std::string message, Client client)
{
	(void)message;
	std::vector<std::string> mode_cmd = Utils::split(message, " \n\t\r");

	if (mode_cmd.size() < 2)
		return;

	std::string channel_name = mode_cmd[1];

	const std::map<std::string, Channel>::iterator &it = getChannels().find(channel_name);

	if (mode_cmd.size() == 2)
	{
		// :harpy.de.SpotChat.org 324 ivo #test :+nt
		// :harpy.de.SpotChat.org 329 ivo #test :1712114531
		std::vector<char> channelModes = it->second.getModes();
		std::string channelModesStr(channelModes.begin(), channelModes.end());
		std::string tmp = ":" + getHostname() + " 324 " + client.getNick() + " " + channel_name + " :+" + channelModesStr + "\r\n";
		SEND(client.getSocket(), tmp, "Error sending MODE message");
		SEND(client.getSocket(), ":" + getHostname() + " 329 " + client.getNick() + " " + channel_name + " :" + it->second.getCreationTime() + "\r\n", "Error sending MODE message");
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
	//int clientSocket = client.getSocket();

	if (modeOption == 'o' && mode_cmd.size() == 4)
	{
		msg = handleOperatorMode(mode_cmd, it, modeFlag, client);
	}
	else if (modeOption == 'i' && mode_cmd.size() == 3)
	{
		handlePrivateAccessMode(it, modeOption, modeFlag, client);
	}
	else if (modeOption == 't' && mode_cmd.size() == 3)
	{
		handleRestrictedTopicMode(it, modeFlag, client, modeOption);
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
	// std::cout << msg << std::endl;
	//send(clientSocket, msg.c_str(), msg.size(), 0);
}

std::string Server::handleOperatorMode(const std::vector<std::string> &mode_cmd, std::map<std::string, Channel>::iterator it, char modeFlag, Client client)
{
	if (!Utils::isValidUser(it->second, mode_cmd[3]))
	{
		return (mode_cmd[3] + " " + it->first + " :User is not in the channel\r\n");
	}
	if (modeFlag == '+')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
		{
			return (mode_cmd[3] + " " + it->first + " :User is already an operator\r\n");
		}
		it->second.AddOperator(it->second.getUsers().find(mode_cmd[3])->first);
		updateChannel(client, mode_cmd[1]);
		return (mode_cmd[3] + " " + it->first + " :User added as an operator\r\n");
	}
	else if (modeFlag == '-')
	{
		if (Utils::isOperator(it->second, mode_cmd[3]))
		{
			it->second.RemoveOperator(mode_cmd[3]);
			updateChannel(client, mode_cmd[1]);
			return (mode_cmd[3] + " " + it->first + " :User removed as an operator\r\n");
		}
		return (mode_cmd[3] + " " + it->first + " :User can't be removed of the operator's list because it's not an operator\r\n");
	}
	return NULL;
}

void Server::handlePrivateAccessMode(std::map<std::string, Channel>::iterator it, char modeOption, char modeFlag, Client client)
{
	if(modeFlag == '+')
	{
		if (it->second.getInvisibility())
		{
			return (Utils::logMessage("Channel is already private", 0), void());
		}
		it->second.setInvisibility(true);
		it->second.setNewMode(modeOption);
		SEND(client.getSocket(),":" + client.getNick() + " MODE " + it->first + " +" + modeOption, "Error sending MODE message");
		return (Utils::logMessage("Channel is now private", 0), void());
	}
	else if (modeFlag == '-')
	{
		if (it->second.getInvisibility())
		{
			it->second.setInvisibility(false);
			it->second.deleteMode(modeOption);
			return (Utils::logMessage("Channel is now public", 0), void());
		}
		return (Utils::logMessage("Channel is already public", 0), void());
	}
}

void Server::handleRestrictedTopicMode(std::map<std::string, Channel>::iterator it, char modeFlag, Client client, char modeOption)
{
	(void)client;
	(void)modeOption;
	std::string channelName = it->second.getName();
	if(modeFlag == '+')
	{
		// quando o canal e criado de forma privada o topico ja e restrito? 
		if (it->second.getRestrictedTopic())
		{
			return (Utils::logMessage("Channel topic is already restricted", 0), void());
		}
		it->second.setRestrictedTopic(true);
		it->second.setNewMode('t');
		//SEND(client.getSocket(),":" + getHostname() + " MODE " + channelName + " +" + modeOption, "Error sending MODE message");
		return (Utils::logMessage("Channel topic is now restricted", 0), void());
	}
	else if (modeFlag == '-')
	{
		if(it->second.getRestrictedTopic())
		{
			it->second.setRestrictedTopic(false);
			it->second.deleteMode('t');
			//SEND(client.getSocket(),":" + getHostname() + " MODE " + channelName + " -" + modeOption, "Error sending MODE message");
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

void Server::INVITE(Client client)
{
	if(getInput().size() != 3)
		return ;

	std::string invitedUser = getInput()[1];
	std::string channel = getInput()[2];

	// :dan-!d@localhost INVITE Wiz #test
	std::string msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " INVITE " + invitedUser + " " + channel + "\r\n";
	SEND(client.getSocket(), msg, "Error sending INVITE message");
}
