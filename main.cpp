/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 23:04:15 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ircserv.hpp"

int main(int argc, char* argv[])
{
	Server	server;

	if (argc == 3)
	{
		if (server.start(argv[1]))
		{
			char hostname[256];
			if (gethostname(hostname, sizeof(hostname)) == -1) {
				std::cerr << "Erro ao obter o nome do host." << std::endl;
				return 1;
			}

			std::cout << "Nome do servidor: " << hostname << std::endl;
			server.run();
		}
		close(server.getSocket());
	}

	return 0;
}