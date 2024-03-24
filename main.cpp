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
		if (!isValidPort(argv[1]))
			return 0;
		int port = std::atoi(argv[1]);
		int serverSocket = getServerSocket();
		struct sockaddr_in serverAddress = getServerAddress(port);
		if (serverSocket && bindSocketAddress(serverSocket, serverAddress) && listenConnections(serverSocket))
		{
			getAddressInfo();
			connectClient(serverSocket);
		} 
		close(serverSocket);
	}

	return 0;
}