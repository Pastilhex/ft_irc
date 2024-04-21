/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:32:22 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/21 16:20:08 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ircserv.hpp"

//:Lenovo!ivo@ivo-Yoga-7-14ITL5 PRIVMSG ivo :DCC SEND microshell.c 2130706433 35159 1512
void Server::PRIVMSG(std::string message, Client client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName = "";
	std::string msgToSend = getMessage(message);

	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			break;
		}
		++inputIterator;
	}

	if (channelName[0] != '#' && channelName[0] != '&')
	{
		std::map<std::string, Client> &users = getGlobalUsers();
		std::map<std::string, Client>::iterator user_it = users.find(input[1]);
		if (user_it != users.end())
		{
			if (isDCC_SEND(input[2]))
				DCC_ACCEPT(input[2]);
			SEND(user_it->second.getSocket(), RPL_PRIVMSG(channelName, msgToSend), "Error sending message to user.");
		}
		else
			SEND(client.getSocket(), ERR_NOSUCHNICK(client, input[1]), "Error sending message to user.");
	}
	else
	{
		std::map<std::string, Channel> channels = getChannels();
		std::map<std::string, Channel>::iterator it = channels.find(channelName);
		if (it != channels.end())
		{
			std::map<std::string, Client> &users = it->second.getUsers();
			std::map<std::string, Client>::iterator user_it = users.begin();
			while (user_it != users.end())
			{
				if (user_it->first != client.getNick())
				{
					SEND(user_it->second.getSocket(), RPL_PRIVMSG(channelName, msgToSend), "Error sending message to channel.");
				}
				++user_it;
			}
		}
	}
}

std::string getMessage(std::string message)
{
	int begin = message.find_first_of(":") + 1;
	int end = message.find_first_of("\r\n", begin); //find_last_not_of returns the position of the last character that is not in the string
	return message.substr(begin, end - begin);
}

std::vector<std::string> Server::isDCC_SEND(std::string message)
{
	if (message.find("DCC SEND") != std::string::npos)
	{
		std::vector<std::string> words;
		std::string word;
		int begin = message.find('\001');
		int end = message.find_last_of('\001');
		std::string trimmed = message.substr(begin, end - begin + 1);
		std::stringstream ss(trimmed);
		int i = 0;
		while (ss >> word)
		{
			words.push_back(word.substr(1));
			while (ss >> word)
			{
				if (word[i = word.find('\001')] == '\001')
					word = word.substr(0, i);
				words.push_back(word);
			}
		}
		if (words[0] != "DCC")
			return;
		if (words[1] != "SEND")
			return;
		if (words[2].empty())
			return
		return words;
	}
	return;
}

void Server::DCC_ACCEPT(std::vector<std::string> words)
{
	
}

bool isValidIPAddress(unsigned int ip)
{
    // Converter o número inteiro para uma string
    std::ostringstream oss;
    oss << ((ip >> 24) & 0xFF) << '.' << ((ip >> 16) & 0xFF) << '.' << ((ip >> 8) & 0xFF) << '.' << (ip & 0xFF);
    std::string ipString = oss.str();

    // Verificar se cada octeto está no intervalo válido (0 a 255)
    std::istringstream iss(ipString);
    int octet;
    char dot;
    for (int i = 0; i < 4; ++i) {
        iss >> octet >> dot;
        if (octet < 0 || octet > 255 || dot != '.') {
            return false;
        }
    }
    // Verificar se não há caracteres adicionais após o último octeto
    char c;
    if (iss >> c) {
        return false;
    }
    return true;
}

bool isPort(int port)
{
    return port == 6667;
}

// char buffer[1024]; // Buffer para armazenar os dados recebidos
//     int bytesRead;

//     // Loop para receber dados até que a conexão seja encerrada
//     while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
//         // Processar os dados recebidos, por exemplo, salvar em um arquivo local
//         if (!saveDataToFile("arquivo_recebido.txt", buffer, bytesRead)) {
//             std::cerr << "Erro ao salvar os dados recebidos em um arquivo." << std::endl;
//             return 1;
//         }
//     }

//     if (bytesRead == 0) {
//         std::cout << "Conexão encerrada pelo cliente." << std::endl;
//     } else if (bytesRead == -1) {
//         std::cerr << "Erro ao receber dados do cliente." << std::endl;
//         return 1;
//     }

// // Função para aceitar uma conexão DCC
// void acceptDCC(const std::string& sender, const std::string& fileName, int fileSize) {
//     // Verificar se a conexão está sendo aceita do remetente esperado
//     if (sender == "remetente_esperado") {
//         // Verificar se o tamanho do arquivo é razoável
//         if (fileSize > 0 && fileSize < 1000000) { // Exemplo de verificação de tamanho
//             std::cout << "Aceitando conexão DCC de " << sender << " para o arquivo " << fileName << std::endl;
//             // Aqui você pode iniciar a transferência do arquivo
//         } else {
//             std::cout << "Tamanho do arquivo inválido." << std::endl;
//         }
//     } else {
//         std::cout << "Conexão DCC não autorizada." << std::endl;
//     }
// }

// int main() {
//     // Simulando uma mensagem DCC recebida
//     std::string sender = "remetente_esperado";
//     std::string fileName = "example.txt";
//     int fileSize = 500000; // Exemplo de tamanho de arquivo

//     // Chamando a função para aceitar a conexão DCC
//     acceptDCC(sender, fileName, fileSize);

//     return 0;
// }
