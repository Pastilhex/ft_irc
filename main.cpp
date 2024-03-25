/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 18:51:14 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

int main(int argc, char* argv[])
{
	(void) argv;

	if (argc == 3)
	{
		if (!ft_isValidPort(argv[1]))
			return 0;
		int port = atoi(argv[1]);
		int serverSocket = ft_getServerSocket();
		struct sockaddr_in serverAddress = ft_getServerAddress(port);
		if (serverSocket && ft_bindSocketAddress(serverSocket, serverAddress) && ft_listenConnections(serverSocket))
		{
			ft_getAddressInfo();
			ft_connectClient(serverSocket);
		} 
		close(serverSocket);
	}

	return 0;
}