/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getServerAddress.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 07:40:25 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 09:30:50 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

struct sockaddr_in getServerAddress(int port)
{
	struct sockaddr_in serverAddress;

	std::memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	return serverAddress;
}