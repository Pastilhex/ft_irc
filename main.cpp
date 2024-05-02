/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/05/02 14:25:07 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

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
