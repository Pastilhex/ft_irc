/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:36:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 18:50:51 by ialves-m         ###   ########.fr       */
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
#include <netdb.h>

int		ft_getServerSocket(void);
struct	sockaddr_in ft_getServerAddress(int port);
bool	ft_bindSocketAddress(int& socket, struct sockaddr_in& addr);
bool	ft_listenConnections(int& serverSocket);
bool	ft_isValidPort(char *str);
bool	ft_connectClient(int& serverSocket);
void	ft_getAddressInfo(void);


#endif