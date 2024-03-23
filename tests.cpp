/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:28:47 by ialves-m          #+#    #+#             */
/*   Updated: 2024/03/23 17:32:21 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h> // Para o close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Criação do socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Erro ao criar o socket." << std::endl;
        return 1;
    }

    // Configuração do endereço do servidor
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345); // Porta do servidor
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Ligação do socket ao endereço
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Erro ao ligar o socket ao endereço." << std::endl;
        close(server_socket);
        return 1;
    }

    // Colocar o socket em modo de escuta
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Erro ao colocar o socket em modo de escuta." << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Servidor aguardando conexões..." << std::endl;

    // Loop principal para aceitar conexões
    while (true) {
        // Aceitar conexão de um cliente
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
            close(server_socket);
            return 1;
        }

        // Trabalhar com a conexão do cliente...
        std::cout << "Cliente conectado." << std::endl;

        // Fechar o socket do cliente
        close(client_socket);
    }

    // Fechar o socket do servidor
    close(server_socket);

    return 0;
}
