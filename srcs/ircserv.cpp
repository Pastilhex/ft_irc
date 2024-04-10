/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:07:13 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/10 13:22:23 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

std::string getCurrentDateTime()
{
	char buffer[80];
	std::time_t rawtime;
	std::tm *timeinfo;

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 80, "%H:%M:%S %b %d %Y", timeinfo);

	return std::string(buffer);
}

bool isCMD(std::string message, std::string cmd)
{
	size_t cmdPos = message.find(cmd);

	if (cmdPos == 0 || (cmdPos != std::string::npos && (cmdPos == 1 || (cmdPos > 1 && message[cmdPos - 1] == '\n'))))
		return true;
	return false;
}

std::string getInputCmd(std::string message, std::string cmd)
{
	int msgSize = cmd.size() + 1;
	return message.substr(message.find(cmd) + msgSize, message.find_first_of(" \n\r\t", message.find(cmd) + msgSize) - (message.find(cmd) + msgSize));
}

std::string getInputChannel(std::string message)
{
	int begin = message.find_first_of("#&") + 1;
	int end = message.find_first_of(" \r\n", begin);
	return message.substr(begin, end - begin);
}

std::string getMsgToSend(std::string message)
{
	int begin = message.find_first_of(":") + 1;
	int end = message.find_last_of(" \r\n", begin);
	return message.substr(begin, end - begin);
}

void SEND(int socket, std::string msg, std::string error)
{
	if (send(socket, msg.c_str(), msg.length(), 0) == -1)
		std::cerr << error << std::endl;
	std::cout << msg << std::endl;
}

std::vector<std::string> trimInput(std::string input)
{
	int begin = input.find_first_not_of(" \r\n\t");
	int end = input.find_last_not_of(" \r\n\t");
	std::string trimmed = input.substr(begin, end-begin);
	
	std::vector<std::string> words;
	std::stringstream ss(trimmed);
	std::string word;

	while (ss >> word)
		words.push_back(word);

	return words;
}