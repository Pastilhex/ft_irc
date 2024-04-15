/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/14 10:48:04 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		std::endl(std::cout);
		Utils::logMessage("SIGINT received. Exiting...", EXIT_SUCCESS);
		exit(EXIT_SUCCESS);
	}
}

bool isPassword(char *pass, Server &server)
{
	if (pass == server.getPassword())
		return true;
	else
	{
		Utils::logMessage("Please check your password", 1);
		return false;
	}
}

int main(int argc, char* argv[])
{
	Server	server(argv[2]);
	signal(SIGINT, signalHandler);
	if (argc == 3 && isPassword(argv[2], server))
	{
		if (server.start(argv[1]))
		{
			server.run();
		}
		close(server.getSocket());
	}
	else
		return (Utils::logMessage("Usage: ./ircserv <port> <password>", 1));
	return 0;
}
