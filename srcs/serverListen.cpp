/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listenConnections.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 07:55:39 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 09:29:45 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

bool	ft_listenConnections(int& serverSocket)
{
	if (listen(serverSocket, 5) == -1)
	{
		std::cerr << "Erro ao colocar o socket em modo de escuta." << std::endl;
		close(serverSocket);
		return false;
	}
	return true;
}