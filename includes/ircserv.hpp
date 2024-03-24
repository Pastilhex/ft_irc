/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:36:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 13:38:04 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IRC_
# define _IRC_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int		getServerSocket(void);
struct	sockaddr_in getServerAddress(int port);
bool	bindSocketAddress(int& socket, struct sockaddr_in& addr);
bool	listenConnections(int& serverSocket);
bool	isValidPort(char *str);



#endif