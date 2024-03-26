/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getServerSocket.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 15:18:28 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

/**
 * Criação de um socket TCP IPv4 para o servidor.
 * 
 * AF_INET, indica que estamos a trabalhar com endereços IPv4.
 * SOCK_STREAM, indica que o socket será orientado para conexão, o que significa que será TCP.
 * 
 **/
int ft_getServerSocket(void)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		cerr << "Erro ao criar o socket." << endl;
		return -1;
	}

	// Define o socket do servidor para modo não-bloqueante
	int flags = fcntl(serverSocket, F_GETFL, 0);
	if (flags == -1)
	{
		std::cerr << "Erro ao obter flags do socket do servidor." << std::endl;
		close(serverSocket);
		return 1;
	}

	if (fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		std::cerr << "Erro ao definir modo não-bloqueante para o socket do servidor." << std::endl;
		close(serverSocket);
		return 1;
	}

	return serverSocket;
}