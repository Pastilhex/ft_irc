/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:36:04 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/05 22:21:26 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _IRC_
#define _IRC_

#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

std::string getCurrentDateTime();

#endif
