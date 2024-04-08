#include "../includes/ircserv.hpp"

/**
 * @brief Handles the MODE command for a server.
 *
 * This function is responsible for processing the MODE command received from a client.
 * It checks the validity of the command arguments, verifies if the channel exists,
 * and checks if the client is an operator of the channel. It also handles setting
 * different flags for the channel, such as making it private or setting a user limit.
 *
 * @param message The MODE command message received from the client.
 * @param client The client who sent the MODE command.
 * @return Returns 0 if the command is successfully processed, otherwise returns an error code.
 */

static bool isOperator(std::string nickname, std::vector<std::string> operators)
{
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == nickname)
			return true;
	}
	return false;
}

void Server::MODE(std::string message, Client client)
{
	(void)client;
	std::vector<std::string> mode_cmd = Utils::split(message, " \n\t\r");
	std::string channel_name = mode_cmd[1].substr(1);
	std::map<std::string, Channel>::iterator it = getChannels().find(channel_name);

	// verificar se o comando tem argumentos suficientes
	std::vector<std::string> operators = it->second.getOperators();
	if (mode_cmd.size() < 3)
	{
		// if (it->second.getOperators().size() == 1 && it->second.getOperators()[0].substr(1) == client.getNick())
		// 	mode_cmd.push_back("+o");
		// else
		return (Utils::logMessage("", 1), void());
		// return (Utils::logMessage("Not enough arguments", 1), void());
	}
	// verificar se o canal existe
	if (channel_name.empty() || it == getChannels().end())
	{
		return (Utils::logMessage("Channel not found", 1), void());
	}
	// verificar se a flag valida
	for (size_t i = 2; i < mode_cmd.size(); i++)
	{
		if (mode_cmd[i] == "+o")
		{
			// verificar se o comando tem argumentos suficientes
			if (mode_cmd.size() != 4)
				return (Utils::logMessage("Invalid number of arguments", 1), void());

			// verificar se o user a ser promovido a operador existe
			else if (!Utils::isValidUser(it->second, mode_cmd[3]))
				return (Utils::logMessage("Invalid user", 1), void());

			// verificar se o user a ser promovido e o proprio operador
			else if (mode_cmd[3] == it->second.getName())
				return (Utils::logMessage("You can't set yourself as operator", 1), void());

			// verificar se o user ja e operador
			else if (isOperator(mode_cmd[3], operators))
				return (Utils::logMessage("User is already an operator", 1), void());

			else
			{
				// it->second.setNewOperator(it->second.getUsers().find(mode_cmd[3])->second);
				Utils::logMessage("User is now an operator", 0);
				break;
			}
		}
		else if (!Utils::isValidFlag(mode_cmd[i]))
		{
			return (Utils::logMessage("Not a valid flag", 1), void());
		}
	}
	if (mode_cmd[2] == "+i")
		Utils::logMessage("Channel is now private", 0);
	else if (mode_cmd[2] == "+t")
		Utils::logMessage("Channel topic is now restricted", 0);
	else if (mode_cmd[2] == "+k")
		Utils::logMessage("Channel key is now set", 0);
	else if (mode_cmd[2] == "+l")
		Utils::logMessage("User limit is now set", 0);
	else
		Utils::logMessage("Flag validation went wrong", 1);
}
