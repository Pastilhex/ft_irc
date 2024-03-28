/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:36:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 22:13:22 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IRC_
# define _IRC_

using namespace std;

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <fcntl.h>
#include <string>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <map>

void	sendWelcome(int clientSocket);

#endif