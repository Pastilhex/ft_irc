/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/06 22:16:18 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		Utils::logMessage("SIGINT received. Exiting...", EXIT_SUCCESS);
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char* argv[])
{
	Server	server("pass");
	signal(SIGINT, signalHandler);
	if (argc == 3)
	{
		if (server.start(argv[1]))
		{
			server.run();
		}
		close(server.getSocket());
	}
	return 0;
}