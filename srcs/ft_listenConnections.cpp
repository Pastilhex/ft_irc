/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listenConnections.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 07:55:39 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 12:18:11 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

bool	ft_listenConnections(int& serverSocket)
{
	if (listen(serverSocket, 5) == -1)
	{
		cerr << "Erro ao colocar o socket em modo de escuta." << endl;
		close(serverSocket);
		return false;
	}
	return true;
}