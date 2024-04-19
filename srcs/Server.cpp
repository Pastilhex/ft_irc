/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/19 12:35:36 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Server::Server(std::string password)
{
	this->_password = password;
}

std::map<std::string, Client> &Server::getGlobalUsers(void)
{
	return this->_globalUsers;
}

int Server::getSocket(void)
{
	return this->_socket;
}

std::string Server::getHostname(void) const
{
	return this->_hostname;
}

struct sockaddr_in Server::getAddress(void)
{
	return this->_address;
}

std::string Server::getPassword(void)
{
	return this->_password;
}

std::vector<std::string> Server::getInput(void)
{
	return this->_input;
}

std::map<std::string, Channel> &Server::getChannels(void)
{
	return this->_channels;
}

void Server::setNewChannel(std::string channel_name, Channel channel)
{
	_channels.insert(std::make_pair(channel_name, channel));
}

void Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

void Server::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

void Server::setInput(std::string message)
{
	this->_input = trimInput(message);
}

bool Server::isValidPort(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}

int Server::createSocket(void)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		std::cerr << "Erro ao criar o socket." << std::endl;
		return -1;
	}

	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Erro ao definir modo não-bloqueante para o socket do servidor." << std::endl;
		close(serverSocket);
		return -1;
	}

	int optval = 1;
	if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) == -1)
	{
		std::cerr << "Erro ao reutilizar o socket do servidor." << std::endl;
		return -1;
	}

	return serverSocket;
}

void Server::createHostname(void)
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		std::cerr << "Erro ao obter o nome do host." << std::endl;
		return;
	}
	this->setHostname((std::string)hostname);
	std::cout << "Nome do servidor: " << hostname << std::endl;
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

std::string Server::getAddressIP(void)
{
	char hostname[256];
	char *ipAddress = NULL;

	// Obtém o nome do host local
	if (gethostname(hostname, sizeof(hostname)) != 0)
	{
		std::cerr << "Erro ao obter o nome do host local" << std::endl;
	}

	// Obtém as informações do host local
	struct hostent *host = gethostbyname(hostname);
	if (host == NULL)
	{
		std::cerr << "Erro ao obter informações do host local" << std::endl;
	}

	// Extrai o endereço IPv4 local do host
	struct in_addr **addr_list = reinterpret_cast<struct in_addr **>(host->h_addr_list);
	if (addr_list[0] != NULL)
	{
		// Converte o endereço IP local para uma string
		ipAddress = inet_ntoa(*addr_list[0]);
	}
	else
	{
		std::cerr << "Nenhum endereço IP encontrado para o host local" << std::endl;
	}
	return ipAddress;
}

bool Server::addClientToGlobalUsers(Client client)
{
	typedef std::map<std::string, Client> ClientMap;
	std::pair<ClientMap::iterator, bool> result = this->_globalUsers.insert(std::make_pair(client.getNick(), client));

	if (result.second)
		return true;
	else
		return false;
}

void Server::removeClientFromGlobalUsers(Client client)
{
	std::map<std::string, Client>::iterator it_begin = this->_globalUsers.begin();
	std::map<std::string, Client>::iterator it_end = this->_globalUsers.end();
	for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
	{
		if (client.getNick() == it->first)
		{
			this->_globalUsers.erase(it);
		}
	}
}

/* Client &Server::getClientBySocket(int socket)
{
	std::map<std::string, Client>::iterator it_begin = _globalUsers.begin();
	std::map<std::string, Client>::iterator it_end = _globalUsers.end();
	for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
	{
		if (it->second.getSocket() == socket)
			return it->second;
	}
	throw std::runtime_error("Cliente não encontrado");
} */

bool Server::isUserInvited(std::string user, std::string channelName)
{
	std::map<std::string, Channel> &channels = this->getChannels();
	std::map<std::string, Channel>::iterator ch = channels.find(channelName);
	if (ch != channels.end())
	{
		std::vector<std::string> &invitedList = ch->second.getInvitedUsers();
		std::vector<std::string>::iterator iv = invitedList.begin();
		while (iv != invitedList.end())
		{
			if (*iv == user)
				return true;
			++iv;
		}
	}
	return false;
}

bool Server::start(char *str)
{
	if (!isValidPort(str))
		return false;
	this->_port = atoi(str);
	this->_socket = createSocket();
	this->_address = createAddress(this->_port);
	return true;
}

bool Server::bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		std::cerr << "Erro ao associar o socket ao endereço." << std::endl;
		close(serverSocket);
		return false;
	}
	return true;
}

bool Server::checkConnections(const int &serverSocket)
{
	if (listen(serverSocket, 5) == -1)
	{
		std::cerr << "Erro ao colocar o socket em modo de escuta." << std::endl;
		close(serverSocket);
		return false;
	}
	return true;
}

void Server::connectClient(const int &serverSocket)
{
	std::vector<pollfd> fds;

	this->serverPoll.fd = serverSocket;
	this->serverPoll.events = POLLIN;
	this->serverPoll.revents = 0;

	fds.push_back(this->serverPoll);

	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity == -1)
		{
			std::cerr << "Erro ao chamar poll()." << std::endl;
			break;
		}
		createNewClient(fds, serverSocket);
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				Client client;

				std::map<std::string, Client>::iterator it_begin = _globalUsers.begin();
				std::map<std::string, Client>::iterator it_end = _globalUsers.end();
				for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
				{
					if (it->second.getSocket() == fds[i].fd)
						client = it->second;
				}
				if (client.getSocket() == 0)
					throw std::runtime_error("Cliente não encontrado");

				char buffer[1024];
				int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead == -1)
				{
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				}
				else if (bytesRead == 0)
				{
					std::cout << "Conexão fechada pelo cliente." << std::endl;
					std::map<std::string, Channel> &channels = this->getChannels();
					std::map<std::string, Channel>::iterator ch = channels.begin();

					std::map<std::string, Client> &globalUsers = this->getGlobalUsers();
					std::map<std::string, Client>::iterator gb = globalUsers.begin();
					while (gb != globalUsers.end())
					{
						if (gb->first == client.getNick())
							globalUsers.erase(gb++);
						else
							++gb;
					}

					while (ch != channels.end())
					{
						std::map<std::string, Client> &users = ch->second.getUsers();
						std::map<std::string, Client>::iterator us = users.begin();
						while (us != users.end())
						{
							if (us->first == client.getNick())
								users.erase(us++);
							else
								++us;
						}
						std::vector<std::string> &operators = ch->second.getOperators();
						std::vector<std::string>::iterator op = operators.begin();
						while (op != operators.end())
						{
							if (*op == client.getNick())
								operators.erase(op++);
							else
								++op;
						}
						std::vector<std::string> &invitedUsers = ch->second.getInvitedUsers();
						std::vector<std::string>::iterator in = invitedUsers.begin();
						while (in != invitedUsers.end())
						{
							if (*in == client.getNick())
								invitedUsers.erase(in++);
							else
								++in;
						}
						++ch;
					}
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
				{
					processMsg(client, fds, buffer, bytesRead, i);
				}
			}
		}
	}
	close(serverSocket);
}

void Server::createNewClient(std::vector<pollfd> &fds, const int &serverSocket)
{
	// verifica se a ligação estabelicida através do poll() é para o server(novo client) ou para um client(client já conectado)
	if (fds[0].revents & POLLIN)
	{
		Client client;
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof(clientAddress);

		// declaração de um novo client_fd (struct do tipo pollfd)
		client.setPoll_fd(accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize));
		if (client.getSocket() == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			return;
		}
		client.setPoll_events();
		client.setPoll_revents();

		// adicionamos o novo client_fd ao vector<pollfd>
		fds.push_back(client.getClientPoll());

		char buffer[1024];
		while (client.getNick().empty() || client.getUsername().empty() || client.getTmpPassword().empty() || client.getTmpPassword() != this->getPassword())
		{
			int bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0);
			std::string message(buffer, bytesRead);

			if (message.find("CAP LS") != std::string::npos)
				SEND(client.getSocket(), ":* CAP * LS :42Porto Ft_IRCv1.0\r\n", "Error sending CAP LS message to client");

			std::cout << "<< " + message << std::endl;
			client.getClientLoginData(buffer, bytesRead, getGlobalUsers(), getHostname());

			if (!client.getTmpPassword().empty() && client.getTmpPassword() != this->getPassword())
			{
				SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error while login");
				continue; // Skip to the next iteration of the outer loop
			}
		}

		// Adiciona o client à lista global de usuarios
		if (!addClientToGlobalUsers(client))
			if (client.getSocket() == -1)
			{
				std::cerr << "Erro ao aceitar a conexão com o nick: " + client.getNick() + "." << std::endl;
				close(serverSocket);
				return;
			}

		if (!client.getNick().empty() && !client.getUsername().empty())
			sendWelcome(client.getSocket(), client);
	}
}

void Server::processMsg(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i)
{
	std::string message(buffer, bytesRead);
	
	std::vector<std::string> splitMessage = Utils::splitVector(message, "\n");

	while (splitMessage.size())
	{

		if (message.empty() || message.compare("\n") == 0)
			return;
		else
			setInput(message);
		std::cout << RED << "<< " << RED + message;

		if (message.find("CAP END") != std::string::npos)
			SEND(fds[i].fd, ":* CAP * END\r\n", "Error sending CAP LS message to client");

		else if (isCMD(message, "PING"))
		{
			SEND(client.getSocket(), RPL_PONG(client.getNick(), client.getUsername(), getInput()[1]), "Error sending PONG message");
		}

		else if (isCMD(message, "PRIVMSG"))
		{
			PRIVMSG(message, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "NICK") || isCMD(message, "USER ") || isCMD(message, "PASS"))
		{
			client.getClientLoginData(buffer, bytesRead, getGlobalUsers(), getHostname());
			WHO(client.getSocket(), client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "MODE"))
		{
			MODE(message, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "WHO"))
		{
			WHO(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "LIST"))
		{
			LIST(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "JOIN"))
		{
			JOIN(fds[i].fd, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "PART"))
		{
			PART(message, client);
			fds[i].revents = 0;
		}
		else if (message.find("QUIT") != std::string::npos)
		{
			close(fds[i].fd); // Fecha a conexão com o cliente
			// Envie uma mensagem de saída para todos os usuarios dos canais que o cliente estava
			// Remova o cliente de todos os canais
			fds.erase(fds.begin() + i); // Remova o cliente do vector<pollfd>
		}
		else if (isCMD(message, "KICK"))
		{
			KICK(message, client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "TOPIC"))
		{
			TOPIC(client);
			fds[i].revents = 0;
		}
		else if (isCMD(message, "INVITE"))
		{
			INVITE(client);
			fds[i].revents = 0;
		}
		splitMessage.erase(splitMessage.begin());
	}
}


bool Server::run(void)
{
	if (getSocket() && bindSocket(getSocket(), getAddress()) && checkConnections(getSocket()))
	{
		createHostname();
		std::cout << "Endereço IP do servidor: " + getAddressIP() << std::endl;
		connectClient(getSocket());
		return true;
	}
	return false;
}

std::string Server::getOpNick(std::string &channelName, std::string clientName)
{
	std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
	if (channelIt != _channels.end())
	{
		Channel &channel = channelIt->second;
		for (size_t i = 0; i < channel.getOperators().size(); ++i)
		{
			size_t found = channel.getOperators()[i].find(clientName, 1);
			if (found != std::string::npos)
				return channel.getOperators()[i];
		}
	}
	return "";
}

std::string Server::getClientNick(std::string &channelName, std::string &clientName)
{
	std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
	if (channelIt != _channels.end())
	{
		Channel &channel = channelIt->second;
		std::map<std::string, Client>::iterator clientIt = channel.getUsers().find(clientName);
		if (clientIt != channel.getUsers().end())
		{
			Client &client = clientIt->second;
			return client.getNick();
		}
	}
	return "";
}

void Server::updateChannel(Client client, std::string channelName)
{
	(void)client;
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			WHO(user_it->second.getSocket(), user_it->second);
			++user_it;
		}
		return;
	}
}

void Server::broadcastTOPIC(Client client, std::string channelName)
{
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			SEND(user_it->second.getSocket(), RPL_TOPIC(client, it->second), "Erro ao enviar mensagem de alteração de TOPIC.");
			++user_it;
		}
		return;
	}
}

void Server::broadcastKICK(Client client, std::string kicked, std::string channelName, std::string reason)
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
				SEND(user_it->second.getSocket(), RPL_KICK(client, channelName, kicked, reason), "Error informing all users");
			++user_it;
		}
		return;
	}
}

void Server::sendWelcome(int clientSocket, Client &client)
{
	std::string welcome;
	welcome += ":" + getHostname() + " 001 " + client.getNick() + " :Welcome to the Internet Relay Network, " + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressIP() + "\r\n";
	welcome += ":" + getHostname() + " 002 " + client.getNick() + " :Your host is " + getHostname() + ", running version FT_IRC_42Porto_v1.0\r\n";
	welcome += ":" + getHostname() + " 003 " + client.getNick() + " :This server was created " + getCurrentDateTime() + "\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :███████╗████████╗    ██╗██████╗  ██████╗\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██╔════╝╚══██╔══╝    ██║██╔══██╗██╔════╝\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :█████╗     ██║       ██║██████╔╝██║     \r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██╔══╝     ██║       ██║██╔══██╗██║     \r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :██║        ██║ ████╗ ██║██║  ██║╚██████╗\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :╚═╝        ╚═╝  ╚══╝ ╚═╝╚═╝  ╚═╝ ╚═════╝\r\n";
	welcome += ":" + getHostname() + " 372 " + client.getNick() + " :Project by:  ialves-m  lpicoli  jhogonca\r\n";
	welcome += ":" + getHostname() + " 376 " + client.getNick() + " :End of /MOTD command.\r\n";
	if (send(clientSocket, welcome.c_str(), welcome.length(), 0) == -1)
	{
		std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
	}
}

std::vector<std::string> Server::trimInput(std::string msg)
{
	int begin = msg.find_first_not_of(" \r\n\t");
	int end = msg.find_last_not_of(" \r\n\t,");
	std::string trimmed = msg.substr(begin, end - begin + 1);

	// Substituir vírgulas por espaços
	for (size_t i = 0; i < trimmed.size(); ++i)
	{
		if (trimmed[i] == ',')
		{
			trimmed[i] = ' ';
		}
	}

	std::vector<std::string> words;
	std::string word;
	std::stringstream ss(trimmed);
	while (ss >> word)
	{
		if (word[0] == ':')
		{
			words.push_back(word);
			while (ss >> word)
			{
				words.back() += " ";
				words.back() += word;
			}
		}
		else
			words.push_back(word);
	}

	return words;
}

bool Server::checkInput(std::vector<std::string> input, Client client)
{
	bool errorDetected = true;
	std::vector<std::string> cmd;

	cmd.push_back("LIST");
	cmd.push_back("TOPIC");
	cmd.push_back("JOIN");
	cmd.push_back("WHO");
	cmd.push_back("PART");
	cmd.push_back("MODE");
	cmd.push_back("KICK");
	cmd.push_back("INVITE");
	cmd.push_back("PRIVMSG");

	for (size_t i = 0; i < cmd.size(); ++i)
	{
		if (input[0] == cmd[i])
			errorDetected = false;
	}

	if (input.size() > 1 && input[1][0] != '#' && input[1][0] != '&')
	{
		SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, input[1]), "Error while sending wrong channel");
		return true;
	}

	return errorDetected;
}
