#include "../includes/Utils.hpp"

int Utils::logMessage(const std::string &message, int level)
{
	std::string logTime = getCurrentDateTimeAsString();

	if (level == EXIT_SUCCESS)
	{
		std::cout << GREEN << "[INFO] " << RESET
				  << logTime << " " << message << std::endl;
	}
	else
	{
		std::cerr << RED << "[ERROR] " << RESET
				  << getCurrentDateTimeAsString() << " " << message << RESET << std::endl;
	}
	return level;
}

bool Utils::hasNonDigits(const std::string &str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
		{
			return true;
		}
	}
	return false;
}

void Utils::signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		logMessage("SIGINT received. Exiting...", EXIT_SUCCESS);
		exit(EXIT_SUCCESS);
	}
}

bool Utils::isValidPassword(const std::string &str)
{
	if (str.empty() || str.length() > 9)
	{
		logMessage("Password is empty or too long.", 1);
		return false;
	}
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isalnum(str[i]))
		{
			logMessage("Password contains invalid characters.", 1);
			return false;
		}
	}
	return true;
}

bool Utils::isValidPort(const std::string &portStr)
{
	if (portStr.empty() || hasNonDigits(portStr))
	{
		logMessage("Port is empty or contains non-digits.", 1);
		return false;
	}
	int port = std::atoi(portStr.c_str());
	if (port <= 1024 || port >= 65535)
	{
		logMessage("Port is out of range.", 1);
		return false;
	}
	return true;
}

bool Utils::isValidInput(const std::string &port, const std::string &password)
{
	return (isValidPort(port) && isValidPassword(password));
}

std::string Utils::getCurrentDateTimeAsString()
{
	char buffer[80];
	struct tm *localTime;

	std::time_t currentTime = std::time(NULL);
	localTime = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	return std::string(buffer);
}

std::string Utils::findWord(const std::string &keyword, const std::string &delimiter, const std::string &text)
{
	std::string foundWord;
	size_t pos = text.find(keyword);

	if (pos != std::string::npos)
	{
		pos += keyword.length();
		size_t end = text.find_first_of(delimiter, pos);
		if (end != std::string::npos)
		{
			foundWord = text.substr(pos, end - pos);
		}
		else
		{
			foundWord = text.substr(pos);
		}
	}
	return foundWord;
}

std::vector<std::string> Utils::split(const std::string &str, const std::string &delimiters)
{
	std::vector<std::string> tokens;									   // Vetor para armazenar os tokens resultantes
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0); // Encontra a primeira posição não de delimitadores

	while (lastPos != std::string::npos)
	{
		// Encontra a próxima posição de delimitador após lastPos
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);
		// Extrai o token entre lastPos e pos
		if (pos == std::string::npos)
		{
			tokens.push_back(str.substr(lastPos)); // Se não houver mais delimitadores, extrai o restante da string
			break;
		}
		else
		{
			tokens.push_back(str.substr(lastPos, pos - lastPos)); // Extrai o token
		}
		// Encontra a próxima posição não de delimitadores após pos
		lastPos = str.find_first_not_of(delimiters, pos);
	}

	return tokens;
}

bool Utils::isValidUser(Channel channel, std::string nickname)
{
	if (channel.getUsers().find(nickname) != channel.getUsers().end())
		return true;
	return false;
}

bool Utils::isOperator(Channel channel, std::string nickname)
{
	std::vector<std::string> operators = channel.getOperators();
	for (size_t i = 0; i < operators.size(); i++)
	{
		if (operators[i] == nickname)
			return true;
	}
	return false;
}

bool Utils::isValidFlag(char flag)
{
	if(flag == 'k' || flag == 'l' || flag == 'i' || flag == 't' || flag == 'o')
		return true;
	return false;
}