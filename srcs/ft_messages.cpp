/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_messages.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:07:13 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 17:24:34 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

/**
 * @brief Envia uma mensagem de boas-vindas para o cliente.
 *
 * Esta função envia uma mensagem de boas-vindas para o cliente conectado ao servidor.
 * A mensagem contém informações sobre o servidor e uma saudação ao cliente.
 *
 * @param clientSocket O descritor de arquivo do socket do cliente.
 */
void sendWelcome(int clientSocket)
{
    // Cria uma string de boas-vindas
    std::string welcome = ":localhost 001 pastilhex :Welcome to the Internet Relay Chat Network, pastilhex!\n";
    //welcome += "Este é um servidor de exemplo. Divirta-se!\r\n";

    // Envia a mensagem de boas-vindas para o cliente
    if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
	{
        std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
    }
}