/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_messages.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:07:13 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/29 00:05:05 by ialves-m         ###   ########.fr       */
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
void	Server::sendWelcome(int clientSocket, Client &client)
{
    std::string welcome = ":localhost 001 pastilhex :Welcome to the Internet Relay Network, " + getHostname() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressInfo() + "\r\n";

	// 001: Welcome to the Internet Relay Network, [seu_nick]!user@host
	// 002: Your host is irc.server.com, running version UnrealIRCd-5.2.1
	// 003: This server was created [data]
	// 004: irc.server.com UnrealIRCd-5.2.1 [modos de servidor suportados]
	// 005: [lista de recursos suportados pelo servidor]
	// 375: - irc.server.com Message of the Day -
	// 375: Welcome to irc.server.com, the best IRC network out there!
	// 375: Rules: No spamming, no flooding, be respectful to others.
	// 376: End of /MOTD command.

	// * Connected. Now logging in.
	// * *** Looking up your ident...
	// * *** Looking up your hostname...
	// * *** Could not resolve your hostname: Domain not found; using your IP address (188.250.216.53) instead.
	// * Capabilities supported: inspircd.org/poison inspircd.org/standard-replies multi-prefix setname userhost-in-names 
	// * Capabilities requested: multi-prefix setname userhost-in-names 
	// * *** If you are having problems connecting due to registration timeouts type /quote PONG xaAJQBRIW~ or /raw PONG xaAJQBRIW~ now.
	// * Capabilities acknowledged: multi-prefix setname userhost-in-names
	// * *** Ident lookup timed out, using ~ivo instead.

	// * Welcome to the ChatJunkies IRC Network pastilhex!~ivo@188.250.216.53
	// * Your host is chatjunkies.org, running version InspIRCd-3
	// * This server was created 19:20:48 Jun 16 2023
	
    if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
	{
        std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
    }
}