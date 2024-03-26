/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getServerSocket.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 16:05:06 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

/**
 * @brief Função para obter o socket do servidor.
 *
 * Esta função cria e configura um socket do servidor.
 *
 * @return Retorna o descritor de arquivo do socket do servidor se bem-sucedido,
 *         -1 se ocorrer um erro ao criar o socket,
 *         1 se ocorrer um erro ao definir o modo não-bloqueante para o socket.
 */
int ft_getServerSocket(void)
{
    // Cria um socket TCP IPv4
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Verifica se ocorreu um erro ao criar o socket
    if (serverSocket == -1)
    {
        cerr << "Erro ao criar o socket." << endl;
        return -1; // Retorna -1 em caso de erro
    }

    // Define o socket para modo não-bloqueante
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Erro ao definir modo não-bloqueante para o socket do servidor." << std::endl;
        close(serverSocket);
        return 1; // Retorna 1 em caso de erro ao definir o modo não-bloqueante
    }

    return serverSocket; // Retorna o descritor de arquivo do socket do servidor
}