/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:36:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/24 20:45:34 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IRC_
#define _IRC_

// Global variable
extern bool server_shutdown;

// Colors
#define RESET "\033[0m"
#define RED "\033[1;31;1m"
#define GREEN "\033[1;32;1m"
#define YELLOW "\033[1;33;1m"
#define BLUE "\033[1;34;1m"
#define WHITE "\033[1;37;1m"
# define BOLD_GREEN		"\001\033[1;92m\002"

#include <iostream>
#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sstream>
#include <sys/epoll.h>

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "Macros.hpp"
#include "Bot.hpp"

std::string getCurrentDateTime();
bool isCMD(std::string message, std::string cmd);
std::string getInputCmd(std::string message, std::string cmd);
std::string getInputChannel(std::string message);
std::string getMsgToSend(std::string message);
std::string getMessage(std::string message);
void SEND(int socket, std::string msg, std::string error);

#endif
