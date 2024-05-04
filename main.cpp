/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/04 01:51:58 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

bool server_shutdown = false;

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		server_shutdown = true;
		Utils::logMessage("Server shutting down...", EXIT_SUCCESS);
	}
}

int main(int argc, char* argv[])
{
	Server server;
	
	signal(SIGINT, signalHandler);
	if (Utils::inputValidation(argc, argv) == false)
		return (EXIT_FAILURE);
	if (server.start(argv[1], argv[2]) == false)
		return (Utils::logMessage("Error starting server", EXIT_FAILURE));
	if (server.run() == false)
		return (Utils::logMessage("Error running server", EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
