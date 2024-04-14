/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/14 18:06:21 by ialves-m         ###   ########.fr       */
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

void ft_print(std::string str)
{
	std::cout << str << std::endl;
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

void Server::setChannel(std::string channel_name, bool state)
{
	_channels.insert(std::make_pair(channel_name, Channel(channel_name, state)));
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

Client &Server::getClientBySocket(int socket, Client &client)
{
	std::map<std::string, Client>::iterator it_begin = _globalUsers.begin();
	std::map<std::string, Client>::iterator it_end = _globalUsers.end();
	for (std::map<std::string, Client>::iterator &it = it_begin; it != it_end; ++it)
	{
		if (it->second.getSocket() == socket)
			return it->second;
	}
	return client;
}

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
	Client client;
	struct sockaddr_in clientAddress;

	pollfd serverPoll;
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
		isNewClient(fds, serverSocket, clientAddress, client);
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				client = getClientBySocket(fds[i].fd, client);
				char buffer[1024];
				int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytesRead == -1)
					std::cerr << "Erro ao receber dados do cliente." << std::endl;
				else if (bytesRead == 0)
				{
					std::cout << "Conexão fechada pelo cliente." << std::endl;
					std::map<std::string, Channel> &channels = this->getChannels();
					std::map<std::string, Channel>::iterator ch = channels.begin();
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
						std::map<std::string, Client> &globalUsers = this->getGlobalUsers();
						std::map<std::string, Client>::iterator gb = globalUsers.begin();
						while (gb != globalUsers.end())
						{
							if (gb->first == client.getNick())
								globalUsers.erase(gb++);
							else
								++gb;
						}
						++ch;
					}
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
					processMsg(client, fds, buffer, bytesRead, i);
			}
		}
	}
	close(serverSocket);
}

void Server::isNewClient(std::vector<pollfd> &fds, const int &serverSocket, struct sockaddr_in &clientAddress, Client &client)
{
	socklen_t clientAddressSize = sizeof(clientAddress);

	// verifica se a ligação estabelicida através do poll() é para o server(novo client) ou para um client(client já conectado)
	if (fds[0].revents & POLLIN)
	{
		// aceita a nova ligação estabelecida com o server
		client.setSocket(accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize));
		if (client.getSocket() == -1)
		{
			std::cerr << "Erro ao aceitar conexão do cliente." << std::endl;
			close(serverSocket);
			return;
		}

		// declaração de um novo client_fd (struct do tipo pollfd)
		pollfd clientPoll;
		clientPoll.fd = client.getSocket();
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;

		// Limpa os dados de client antes de preencher com novos dados
		client.setNewClient(client);

		// adicionamos o novo client_fd ao vector<pollfd>
		fds.push_back(clientPoll);

		// guardar msg recebida num buffer
		char buffer[1024];
		while (client.getNick().empty() || client.getUsername().empty())
		{
			int bytesRead = recv(clientPoll.fd, buffer, sizeof(buffer), 0);
			std::string message(buffer, bytesRead);
			if (message.find("CAP LS") != std::string::npos)
				SEND(clientPoll.fd, ":* CAP * LS :42Porto Ft_IRCv1.0\r\n", "Error sending CAP LS message to client");
			std::cout << message << std::endl;
			client.getClientLoginData(buffer, bytesRead);
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
			sendWelcome(clientPoll.fd, client);
	}
}

void Server::processMsg(Client &client, std::vector<pollfd> &fds, char *buffer, int bytesRead, int i)
{
	std::string message(buffer, bytesRead);
	setInput(message);

	if (message.find("CAP END") != std::string::npos)
		SEND(fds[i].fd, ":* CAP * END\r\n", "Error sending CAP LS message to client");

	if (!isCMD(message, "PING"))
		std::cout << ":<< " + message << std::endl;

	if (isCMD(message, "PRIVMSG"))
	{
		PRIVMSG(message, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "NICK") || isCMD(message, "USER ") || isCMD(message, "PASS"))
	{
		client.getClientLoginData(buffer, bytesRead);
		fds[i].revents = 0;
	}
	if (isCMD(message, "MODE"))
	{
		MODE(message, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "WHO"))
	{
		WHO(fds[i].fd, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "LIST"))
	{
		LIST(fds[i].fd, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "JOIN"))
	{
		JOIN(fds[i].fd, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "PART"))
	{
		// Usage: PART [<channel>] [<reason>], leaves the channel, by default the current one
		// PART #canalX :Até logo, pessoal!
		PART(message, client);
		fds[i].revents = 0;
	}
	if (message.find("QUIT") != std::string::npos)
	{
		close(fds[i].fd); // Fecha a conexão com o cliente
		// Envie uma mensagem de saída para todos os usuarios dos canais que o cliente estava
		// Remova o cliente de todos os canais
		fds.erase(fds.begin() + i); // Remova o cliente do vector<pollfd>
	}
	if (isCMD(message, "KICK"))
	{
		KICK(message, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "TOPIC"))
	{
		TOPIC(fds[i].fd, client, message);
		fds[i].revents = 0;
	}
	if (isCMD(message, "INVITE"))
	{
		INVITE(client);
		fds[i].revents = 0;
	}
}

void Server::TOPIC(int clientSocket, Client &client, std::string message)
{
	(void)clientSocket;

	std::string channelName = getInputCmd(message, "TOPIC ");
	if (getInput().size() <= 3) // ERR_NEEDMOREPARAMS (461)
	{
		bool isChannelOk = ((!getInput()[1].empty()) ? (getInput()[1][0] == '#' || getInput()[1][0] == '&') ? true : false : false);
		bool isNickOk = (!getInput()[2].empty()) ? true : false;
		if (!isChannelOk || !isNickOk)
		{
			std::string reason = ":" + getHostname() + " 461 " + client.getNick() + " " + ((isChannelOk) ? getInput()[1] : "") + " :Not enough parameters\r\n";
			SEND(client.getSocket(), reason, "Erro ao enviar mensagem de KICK por falta de argumentos");
			return;
		}
	}

	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);

	bool isUserOp = false;
	bool isOpenTopic = it->second.getInvisibility();

	std::vector<std::string> &operators = it->second.getOperators();
	std::vector<std::string>::iterator op = operators.begin();
	while (op != operators.end())
	{
		if ("@" + client.getNick() == *op) // quem esta a aceder ao TOPIC é Operador ?
			isUserOp = true;
		++op;
	}

	if (it != channels.end())
	{
		if (isOpenTopic || isUserOp)
		{
			std::string updateTopic = ":" + getHostname() + " TOPIC " + getInputCmd(message, "KICK") + " :" + /*topic*/ +"\r\n";
			SEND(client.getSocket(), updateTopic, "Erro ao enviar mensagem de alteração de TOPIC.");
		}
	}
}

void Server::PRIVMSG(std::string message, Client client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName;

	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			break;
		}
		++inputIterator;
	}

	std::string msgToSend = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " PRIVMSG " + channelName + " :";
	msgToSend += getMsgToSend(message) + "\r\n";

	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			if (user_it->first != client.getNick() && (send(user_it->second.getSocket(), msgToSend.c_str(), msgToSend.length(), 0) == -1))
			{
				std::cerr << "Erro ao enviar mensagem para o canal." << std::endl;
			}
			++user_it;
		}
	}
}

void Server::LIST(int clientSocket, Client &client)
{
	std::map<std::string, Channel> channels = this->getChannels();
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); ++it)
	{
		std::string channelName = it->first;
		size_t pos = channelName.find('\n');
		if (pos != std::string::npos)
		{
			channelName.erase(pos, 1);
		}
		int nbrUser = it->second.getNbrUsers();
		char nbrUserStr[20];				// Tamanho suficiente para armazenar um inteiro
		sprintf(nbrUserStr, "%d", nbrUser); // Formatar o inteiro como uma string
		std::string channel = RPL_LIST(client, nbrUserStr, it);
		channel += RPL_LISTEND(client);
		SEND(clientSocket, channel, "Error updating LIST");
	}
}

void Server::JOIN(int clientSocket, Client &client)
{
	std::vector<std::string> input = getInput();
	std::vector<std::string>::iterator inputIterator = input.begin();
	std::string channelName;
	while (inputIterator != input.end())
	{
		if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
		{
			channelName = *inputIterator;
			std::map<std::string, Channel> &channels = getChannels();
			std::map<std::string, Channel>::iterator it = channels.find(channelName);
			
			std::vector<char> mode = it->second.getModes();
			std::vector<char>::iterator mode_it = mode.begin();
			for (; mode_it != mode.end(); ++mode_it)
			{
				if (*mode_it == 'i')
				{
					if (isUserInvited(client.getNick(), channelName))
					{
						it->second.RemoveInvited(client.getNick());
						break;
					}
					else
					{
						SEND(client.getSocket(), ERR_INVITEONLYCHAN(client, channelName), "Error sending JOIN message with mode +i advise to user");
						return;
					}
				}
			}
			if ((*inputIterator)[0] == '#' || (*inputIterator)[0] == '&')
			{
				for (it = channels.begin(); it != channels.end(); ++it)
				{
					if (it->first == channelName)
					{
						it->second.setNewUser(client);
						updateChannel(client, channelName);
						break;
					}
				}
				if (it == channels.end())
				{
					bool state = (input[1][0] == '#') ? false : true;
					Channel channel = Channel(channelName, state);
					channel.setNewUser(client);
					channel.AddOperator(client.getNick());
					_channels.insert(std::make_pair(channelName, channel)); // Fazer um setter para esta função
					SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
				}
			}
			SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
			WHO(client.getSocket(), client);
			++inputIterator;
		}
		else
			++inputIterator;
	}
}

void Server::WHO(int clientSocket, const Client client)
{
	std::vector<std::string> input = getInput();
	std::string channelName;
	for (size_t i = 0; i < getInput().size(); ++i)
	{
		if (input[i][0] == '#' || input[i][0] == '&')
		{
			channelName = input[i];
			break;
		}
	}
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	bool channelPrivacy = it->second.getModePrivateAccess();
	std::string privacy = (channelPrivacy) ? "@" : "#";
	if (it != channels.end())
	{
		std::string whoMsg = ":" + getHostname() + " 353 " + client.getNick() + " = " + channelName + " :";
		const std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::const_iterator user_it = users.begin();
		while (user_it != users.end())
		{
			std::string nickname = user_it->first;
			std::vector<std::string> opList = it->second.getOperators();
			std::vector<std::string>::iterator op_it = opList.begin();
			while (op_it != opList.end())
			{
				if ((*op_it).find(nickname, 1) != std::string::npos)
				{
					nickname = *op_it;
					break; // Interrompe o loop assim que encontrar uma correspondência
				}
				++op_it;
			}
			whoMsg += nickname;
			if (++user_it != users.end())
				whoMsg += " ";
		}
		whoMsg += "\r\n:" + getHostname() + " 366 " + client.getNick() + " " + channelName + " :End of Names list.\r\n";
		std::cout << whoMsg << std::endl;
		if (send(clientSocket, whoMsg.c_str(), whoMsg.length(), 0) == -1)
		{
			std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
		}
	}
}

void Server::PART(std::string message, Client &client)
{
	std::string channelName = getInputCmd(message, "PART");
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator us = users.begin();
		while (us != users.end())
		{
			if (us->second.getNick() == client.getNick())
			{
				std::string leaveChannel = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressIP() + " PART " + getInputCmd(message, "PART") + "\r\n";
				if (send(client.getSocket(), leaveChannel.c_str(), leaveChannel.length(), 0) == -1)
				{
					std::cerr << "Erro ao enviar mensagem de saída de canal." << std::endl;
				}
				else
				{
					users.erase(us);
					std::cout << leaveChannel << std::endl;
				}

				if (it->second.getNbrUsers() == 0)
				{
					std::string closeChannel = ":" + getHostname() + " PART " + getInputCmd(message, "PART");
					if (send(client.getSocket(), closeChannel.c_str(), closeChannel.length(), 0) == -1)
					{
						std::cerr << "Erro ao enviar mensagem de fecho de canal." << std::endl;
					}
					else
					{
						channels.erase(it);
						std::cout << closeChannel << std::endl;
					}
				}
				else
					updateChannel(client, channelName);
				break;
			}
			++us;
		}
	}
	else
	{
		std::cout << "Canal não encontrado." << std::endl;
	}
}

void Server::KICK(std::string message, Client client)
{
	// KICK #42 ivo :bye
	// :irc.server.com 482 nickname #canal :You're not channel operator
	// std::string reason = ":" + getHostname() + " 461 " + client.getNick() + " " + getInputCmd(message, "KICK") + " :Not enough parameters\r\n";

	bool isKickerOp = false;
	bool isKickedOp = false;

	std::string kickNick = getInput()[2];
	std::string reason = (getInput().size() == 4 && !getInput()[3].empty()) ? getInput()[3] : "";
	std::string channelName = getInputCmd(message, "KICK");

	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		{
			std::vector<std::string> &operators = it->second.getOperators();
			std::vector<std::string>::iterator op = operators.begin();
			while (op != operators.end())
			{
				if ("@" + client.getNick() == *op) // quem esta a kickar é Operador ?
					isKickerOp = true;
				++op;
			}
		}
		{
			std::vector<std::string> &operators = it->second.getOperators();
			std::vector<std::string>::iterator op = operators.begin();
			while (op != operators.end())
			{
				if ("@" + kickNick == *op) // quem esta a ser kickado é Operador ?
					isKickedOp = true;
				++op;
			}
		}

		if (isKickerOp && isKickedOp) // ERR_NOPRIVILEGES (481)
		{
			std::string leaveChannel = ":" + getHostname() + " 481 " + client.getNick() + " " + getInputCmd(message, "KICK") + " :Permission Denied - You're not an IRC operator\r\n";
			SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
			return;
		}
		else if ((!isKickerOp && isKickedOp) || (!isKickerOp && !isKickedOp)) // ERR_CHANOPRIVSNEEDED (482)
		{
			std::string leaveChannel = ":" + getHostname() + " 482 " + client.getNick() + " " + getInputCmd(message, "KICK") + " :You're not channel operator\r\n";
			SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
			return;
		}
		else if (isKickerOp && !isKickedOp)
		{
			std::map<std::string, Client> &users = it->second.getUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->first == kickNick)
				{
					std::string leaveChannel = RPL_KICK(client, channelName, kickNick, reason);
					SEND(client.getSocket(), leaveChannel, "Erro ao enviar mensagem de KICK.");
					users.erase(us);
					WHO(us->second.getSocket(), us->second);
					informAll(client, kickNick, channelName, RPL_KICK(client, channelName, kickNick, reason));
					updateChannel(client, channelName);
					return;
				}
				++us;
			}
		}
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

void Server::informAll(Client client, std::string kicked, std::string channelName, std::string reason)
{
	std::map<std::string, Channel> channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::iterator user_it = users.begin();
		while (user_it != users.end())
		{
			//if (user_it->first != client.getNick())
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
	std::stringstream ss(trimmed);
	std::string word;
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

bool Server::checkInput(std::vector<std::string> input)
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

	if (input[1][0] == '#' || input[1][0] == '&')
		errorDetected = false;

	return errorDetected;
}

void Server::INVITE(Client client)
{
	bool userAvaiable = false;
	bool channelAvaiable = false;
	int invitedFd;

	if (getInput().size() != 3)
		return;
	
	std::string invitedUser = getInput()[1];
	std::string channel = getInput()[2];
	if (!(channel[0] != '#' || channel[0] != '&'))
	{
		SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, channel), "Error sending ERR_NOSUCHCHANNEL (403)");
		return;
	}

	std::map<std::string, Channel> &channels = this->getChannels();
	std::map<std::string, Channel>::iterator ch = channels.begin();
	while (ch != channels.end())
	{
		if (ch->first == channel)
		{
			channelAvaiable = true;
			std::map<std::string, Client> &users = this->getGlobalUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->first == invitedUser)
				{
					userAvaiable = true;
					invitedFd = us->second.getSocket();
					break;
				}
				++us;
			}
			break;
		}
		++ch;
	}

	if (!channelAvaiable)
	{
		SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, channel), "Error sending msg ERR_NOSUCHCHANNEL");
		return;
	}

	if (userAvaiable)
		ch->second.AddInvited(invitedUser);
	else
	{
		SEND(client.getSocket(), ERR_NOSUCHNICK(client, invitedUser), "Error sendinf msg ERR_NOSUCHNICK");
		return;
	}

	std::string msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " INVITE " + invitedUser + " " + channel + "\r\n";
	SEND(invitedFd, msg, "Error sending INVITE message");
}
