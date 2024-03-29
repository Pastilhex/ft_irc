/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testes.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:10:15 by jhogonca          #+#    #+#             */
/*   Updated: 2024/03/28 23:10:15 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cerrno>
#include <cctype>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Client {
private:
	std::string	_nick;
	std::string	_serverPassword;
	std::string	_tmpPassword;
	std::string	_username;

public:
	/* Getters */
	std::string getNick(void);
	std::string getUsername(void);
	std::string getTmpPassword(void);
	
	/* Setters */
	void setNick(std::string nick);
	void setUsername(std::string username);
	void setTmpPassword(std::string pass);

	/* Methods */
	void	getClientLoginData(char* buffer, int bytesRead);

};

std::string Client:: getNick(void)
{
	return (this->_nick);
}

std::string Client::getUsername(void)
{
	return this->_username;
}

std::string Client::getTmpPassword(void)
{
	return this->_tmpPassword;
}

void Client::setNick(std::string nick)
{
	this->_nick = nick;
}

void Client::setUsername(std::string username)
{
	this->_username = username;
}

void Client::setTmpPassword(std::string pass)
{
	this->_tmpPassword = pass;
}

void Client::getClientLoginData(char* buffer, int bytesRead)
{
	std::string message(buffer, bytesRead);
	if ((message.find("NICK") != std::string::npos) || (message.find("USER") != std::string::npos) || (message.find("PASS") != std::string::npos))
	{
		if (message.find("NICK") != std::string::npos)
			setNick(message.substr(message.find("NICK ") + 5, message.find("\r", message.find("NICK ") + 5) - (message.find("NICK ") + 5)));
		if (message.find("USER") != std::string::npos)
			setUsername(message.substr(message.find("USER ") + 5, message.find(" ", message.find("USER ") + 5) - (message.find("USER ") + 5)));
		if (message.find("PASS") != std::string::npos)
			setTmpPassword(message.substr(message.find("PASS ") + 5, message.find("\r", message.find("PASS ") + 5) - (message.find("PASS ") + 5)));
	}
}

class Utils
{
private:
	static bool hasNonDigits(const std::string &str);
	static bool isValidPort(const std::string &portStr);
	static bool isValidPassword(const std::string &str);

public:
	static bool isValidInput(const std::string &password, const std::string &port);
	static std::string getCurrentDateTimeAsString();
	static std::string findWord(const std::string &keyword, const std::string &delimiter, const std::string &text);
	static int logMessage(const std::string &message, int level);
	static void sendWelcome(int clientSocket, Client& client);
};

int Utils::logMessage(const std::string &message, int level)
{
	std::string logTime = getCurrentDateTimeAsString();

	if (level == EXIT_SUCCESS)
		std::cout << "[INFO] " << logTime << " " << message << std::endl;
	else
		std::cerr << "[ERROR] " << getCurrentDateTimeAsString() << " " << message << std::endl;
	return level;
}

bool Utils::hasNonDigits(const std::string &str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
		{
			return true;
		}
	}
	return false;
}

bool Utils::isValidPassword(const std::string &str)
{
	if (str.empty() || str.length() > 9)
	{
		logMessage("Password is empty or too long.", 1);
		return false;
	}
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isalnum(str[i]))
		{
			logMessage("Password contains invalid characters.", 1);
			return false;
		}
	}
	return true;
}

bool Utils::isValidPort(const std::string &portStr)
{
	if (portStr.empty() || hasNonDigits(portStr))
	{
		logMessage("Port is empty or contains non-digits.", 1);
		return false;
	}
	int port = std::atoi(portStr.c_str());
	if (port <= 1024 || port >= 65535)
	{
		logMessage("Port is out of range.", 1);
		return false;
	}
	return true;
}

bool Utils::isValidInput(const std::string &port, const std::string &password)
{
	return (isValidPort(port) && isValidPassword(password));
}

std::string Utils::getCurrentDateTimeAsString()
{
	char buffer[80];
	struct tm *localTime;

	std::time_t currentTime = std::time(NULL);
	localTime = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	return std::string(buffer);
}

std::string Utils::findWord(const std::string &keyword, const std::string &delimiter, const std::string &text)
{
	std::string foundWord;
	size_t pos = text.find(keyword);

	if (pos != std::string::npos)
	{
		pos += keyword.length();
		size_t end = text.find_first_of(delimiter, pos);
		if (end != std::string::npos)
		{
			foundWord = text.substr(pos, end - pos);
		}
		else
		{
			foundWord = text.substr(pos);
		}
	}
	return foundWord;
}

class Server
{
private:
	// Attributes
	int _port;
	int _socket;
	struct sockaddr_in _address;
	std::string _hostname;
	std::string _password;
	// std::map<std::string, Channel> _channels; // <"Canal42":"objeto_canal"

	// Private methods
	bool createSocket(void);
	bool createHostname(void);
	struct sockaddr_in createAddress(int port);
	bool listenSocket(const int &serverSocket);

public:
	// Methods
	bool start(const std::string &port, const std::string &password);
	bool bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress);
	void connectToClient(const int &serverSocket);

	/* Getters */
	int	getSocket(void);
	std::string getHostname(void);

	/* Setters */
	bool setHostname(void);
	void setSocket(int newSocket);
	void setAddress(struct sockaddr_in newAddress);


	void sendWelcome(int clientSocket, Client& client);
	std::string getAddressIP(void);

};

int Server::getSocket(void)
{
	return this->_socket;
}

void Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

std::string	Server::getAddressIP(void)
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		std::cerr << "Erro ao obter o nome do host." << std::endl;
	}

	struct hostent *host_info = gethostbyname(hostname);
	if (host_info == NULL || host_info->h_addr_list[0] == NULL)
	{
		std::cerr << "Erro ao obter o endereço IP." << std::endl;
	}

	char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));
	return ip_address;
}

void	Server::sendWelcome(int clientSocket, Client &client)
{
    std::string welcome = ":localhost 001 pastilhex :Welcome to the Internet Relay Network, " + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressIP() + "\r\n";
	welcome += ":localhost 002 pastilhex :Your host is " + getHostname() + ", running version FT_IRC_42Porto_v1.0\r\n";
	welcome += ":localhost 003 pastilhex :This server was created " + Utils::getCurrentDateTimeAsString() + "\r\n";
	
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
		Utils::logMessage("Erro ao enviar mensagem de boas vindas para o cliente.", EXIT_FAILURE);
    }
}

bool Server::createHostname(void)
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		Utils::logMessage("Erro ao definir o nome do host.", EXIT_FAILURE);
		return false;
	}
	_hostname = hostname;
	return true;
}

bool Server::setHostname(void)
{
	if (!createHostname())
		return false;
	return true;
}

std::string Server::getHostname(void)
{
	return _hostname;
}

struct sockaddr_in Server::createAddress(int port)
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	return serverAddress;
}

bool Server::createSocket(void)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		Utils::logMessage("Erro ao criar o socket.", EXIT_FAILURE);
		return false;
	}
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		Utils::logMessage("Erro ao definir modo não-bloqueante para o socket do servidor.", EXIT_FAILURE);
		close(serverSocket);
		return false;
	}
	int optval = 1;
	if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) == -1)
	{
		Utils::logMessage("Erro ao reutilizar o socket do servidor.", EXIT_FAILURE);
		close(serverSocket);
		return false;
	}
	_socket = serverSocket;
	return true;
}

bool Server::start(const std::string &port, const std::string &password)
{
    this->_password = password;
    this->_port = std::atoi(port.c_str());
    this->_address = createAddress(this->_port);

    if (!setHostname())
        return false;
    if (!createSocket())
        return false;
	if (this->_socket == -1)
        return false;
    if (!bindSocket(this->_socket, this->_address))
        return false;
	if (!listenSocket(this->_socket))
        return false;
    return true;
}

bool Server::listenSocket(const int &serverSocket)
{
	if (listen(serverSocket, 5) == -1)
	{
		Utils::logMessage("Erro ao colocar o socket em modo de escuta.", EXIT_FAILURE);
		close(serverSocket);
		return false;
	}
	return true;
}

bool Server::bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		Utils::logMessage("Erro ao associar o socket ao endereço.", EXIT_FAILURE);
		close(serverSocket);
		return false;
	}
	return true;
}

void	Server::connectToClient(const int& serverSocket)
{
	Client	client;
	bool	welcomeMessage = false;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	pollfd	serverPoll;
	serverPoll.fd = serverSocket;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;

	std::vector<pollfd> fds;
	fds.push_back(serverPoll);
	while (true)
	{
		
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity == -1)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		if (fds[0].revents & POLLIN)
		{
			int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize);
			if (clientSocket == -1)
			{
				 std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
				close(serverSocket);
				return ;
			}
			pollfd clientPoll;
			clientPoll.fd = clientSocket;
			clientPoll.events = POLLIN;
			clientPoll.revents = 0;
			fds.push_back(clientPoll);
			
		}
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				}
				else if (bytesRead == 0)
				{
					std::cout << "Conexão fechada pelo cliente." << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
				{

					std::string message(buffer, bytesRead);
					client.getClientLoginData(buffer, bytesRead);
					if (message.find("LIST") != std::string::npos)
					{
						std::string channel1 = ":localhost 322 pastilhex #canal2 13 :Canal 42\r\n";
						send(fds[i].fd, channel1.c_str(), channel1.size(), 0);
						//std::string endOfList = ":localhost 323 seu_nick :End of /LIST\r\n";
					}
					else if (message.find("JOIN") != std::string::npos)
					{
						std::string channelJoin = ":pastilhex JOIN #canal2\r\n";
						send(fds[i].fd, channelJoin.c_str(), channelJoin.size(), 0);
						// std::string topicMessage = ":localhost 332 pastilhex #canal2 :Descrição do Canal 2\r\n";
    					// send(fds[i].fd, topicMessage.c_str(), topicMessage.size(), 0);
						// std::string modeCommand = "MODE #canal2 +nt\r\n";
						// send(fds[i].fd, modeCommand.c_str(), modeCommand.size(), 0);
					}
					else
					{
						std::cout << "Dados recebidos do cliente: " << std::string(buffer, bytesRead) << std::endl;
					}

					std::string tmpNick = client.getNick(), tmpUser = client.getUsername();
					if (!tmpNick.empty() && !tmpUser.empty() && welcomeMessage == false)
					{
						sendWelcome(fds[i].fd, client);
						welcomeMessage = true;
					}
				}
			}
		}
	}
	close(serverSocket);
}

int main(int ac, char **av)
{
	Server server;

	if (ac != 3)
	{
		return Utils::logMessage("Invalid input.\n\tUsage: ./server <port> <password>", EXIT_FAILURE);
	}
	if (!Utils::isValidInput(av[1], av[2]))
		return EXIT_FAILURE;
	if (!server.start(av[1], av[2]))
		return (Utils::logMessage("Failed to start server.", EXIT_FAILURE));
	Utils::logMessage("Server started successfully.", EXIT_SUCCESS);
	server.connectToClient(server.getSocket());
	return EXIT_SUCCESS;
}
