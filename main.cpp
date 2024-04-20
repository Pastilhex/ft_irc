/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/20 08:19:36 by jhogonca         ###   ########.fr       */
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

int main(int argc, char* argv[])
{
	Server server;
	
	if (Utils::inputValidation(argc, argv) == false)
		return (EXIT_FAILURE);
	if (server.start(argv[1], argv[2]) == false)
		return (Utils::logMessage("Error starting server", EXIT_FAILURE));
	if (server.run() == false)
		return (Utils::logMessage("Error running server", EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
