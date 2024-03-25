/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bindSocketAddress.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 07:47:19 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/24 09:28:59 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

bool	ft_bindSocketAddress(int& serverSocket, struct sockaddr_in& serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		cerr << "Erro ao associar o socket ao endereço." << endl;
		close(serverSocket);
		return false;
	}

	return true;
}