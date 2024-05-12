/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:51:18 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/12 22:53:40 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
int Utils::logMessage(const std::string &message, int level)
{
	std::string logTime = getCurrentDateTimeAsString();
	std::ostream &out = (level == EXIT_FAILURE) ? std::cerr : std::cout;

	switch (level)
	{
	case EXIT_SUCCESS:
		out << GREEN << "[INFO] " << RESET;
		break;
	case 2:
		out << YELLOW << "[CONFIG] " << RESET;
		break;
	default:
		out << RED << "[ERROR] " << RESET;
		break;
	}
	out << logTime << " " << message << std::endl;
	return level;
}

bool Utils::inputValidation(int argc, char *argv[])
{
	void signalHandler(int signum);
	if (argc != 3)
	{
		Utils::logMessage("Usage: ./ircserv <port> <password>", EXIT_FAILURE);
		return (false);
	}
	if (!Utils::isValidInput(argv[1], argv[2]))
		return (false);
	return (true);
}

void Utils::trim(std::string &str)
{
	size_t first = str.find_first_not_of(" ");
	if (first == std::string::npos)
	{
		str.clear();
		return;
	}
	size_t last = str.find_last_not_of(" ");
	str = str.substr(first, last - first + 1);
}

void Utils::printServerInfo(std::string hostname, std::string port, std::string addressIP, std::string pass)
{
	std::cout << "Endereço IP do servidor: "
			  << WHITE << addressIP << std::endl
			  << RESET;
	std::cout << "Hostname do servidor: "
			  << WHITE << hostname << std::endl
			  << RESET;
	std::cout << "Porta do servidor: "
			  << WHITE << port << std::endl
			  << RESET;
	std::cout << "Password do servidor: "
			  << WHITE << pass << std::endl
			  << RESET;
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
	std::vector<std::string> tokens;
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	while (lastPos != std::string::npos)
	{
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);
		if (pos == std::string::npos)
		{
			tokens.push_back(str.substr(lastPos));
			break;
		}
		else
		{
			tokens.push_back(str.substr(lastPos, pos - lastPos));
		}
		lastPos = str.find_first_not_of(delimiters, pos);
	}

	return tokens;
}

std::vector<std::string> Utils::splitVector(const std::string &str, const std::string &delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = str.find_first_of(delimiter, start)) != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
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
		if (operators[i] == "@" + nickname)
			return true;
	}
	return false;
}

bool Utils::isValidFlag(char flag)
{
	if (flag == 'k' || flag == 'l' || flag == 'i' || flag == 't' || flag == 'o')
		return true;
	return false;
}

bool Utils::isValidPort(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}
