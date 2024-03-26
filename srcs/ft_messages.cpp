/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_messages.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:07:13 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/26 13:40:52 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

void sendWelcome(int clientSocket)
{
    std::string welcome = "Bem-vindo ao servidor FT_IRC!\r\n";
    welcome += "Este é um servidor de exemplo. Divirta-se!\r\n";

    if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
	{
        std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
    }
}