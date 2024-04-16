/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/16 18:24:11 by ialves-m         ###   ########.fr       */
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
	if (message.empty() || message.compare("\n") == 0)
		return;
	else
		setInput(message);
	std::cout << RED << "<< " << RED + message;

	if (message.find("CAP END") != std::string::npos)
		SEND(fds[i].fd, ":* CAP * END\r\n", "Error sending CAP LS message to client");

	if (isCMD(message, "PING"))
	{
		SEND(client.getSocket(), RPL_PONG(client.getNick(), client.getUsername(), getInput()[1]), "Error sending PONG message");
	}

	if (isCMD(message, "PRIVMSG"))
	{
		PRIVMSG(message, client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "NICK") || isCMD(message, "USER ") || isCMD(message, "PASS"))
	{
		client.getClientLoginData(buffer, bytesRead, getGlobalUsers(), getHostname());
		WHO(client.getSocket(), client);
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
		TOPIC(client);
		fds[i].revents = 0;
	}
	if (isCMD(message, "INVITE"))
	{
		INVITE(client);
		fds[i].revents = 0;
	}
}

void Server::TOPIC(Client &client)
{
	if (!checkInput(getInput(), client))
	{
		if (getInput().size() <= 3)
		{
			if (getInput().size() == 1)
			{
				SEND(client.getSocket(), ERR_NEEDMOREPARAMS(client, "TOPIC"), "Error sending TOPIC error message");
				return;
			}
			if (getInput().size() == 2 && (getInput()[1].empty() || !(getInput()[1][0] == '#' || getInput()[1][0] == '&')))
			{
				SEND(client.getSocket(), ERR_NOSUCHNICK(client, client.getNick()), "Error sending TOPIC error message");
				return;
			}
			else if (getInput().size() == 2)
			{
				std::string channelName = getInput()[1];
				std::map<std::string, Channel> &channels = getChannels();
				std::map<std::string, Channel>::iterator it = channels.find(channelName);
				if (it != channels.end())
				{
					if (it->first == channelName)
					{
						if (it->second.getTopic().empty())
							SEND(client.getSocket(), RPL_NOTOPIC(client, it->second), "Erro ao enviar mensagem de alteração de TOPIC.");
						else
							SEND(client.getSocket(), RPL_TOPIC(client, it->second), "Error while sending TOPIC message");
					}
				}
				return;
			}
			if (getInput().size() == 3 && (getInput()[1].empty() || (getInput()[1][0] != '#' || getInput()[1][0] != '&')) && getInput()[2].empty())
				SEND(client.getSocket(), ERR_NEEDMOREPARAMS(client, "TOPIC"), "Error sending TOPIC error message");
			else
			{
				std::string channelName = getInput()[1];
				std::map<std::string, Channel> &channels = getChannels();
				std::map<std::string, Channel>::iterator it = channels.find(channelName);

				bool isUserOp = false;
				bool isRestrictedTopic = it->second.getRestrictedTopic();

				std::vector<std::string> &operators = it->second.getOperators();
				std::vector<std::string>::iterator op = operators.begin();
				while (op != operators.end())
				{
					if ("@" + client.getNick() == *op)
						isUserOp = true;
					++op;
				}
				if (it != channels.end())
				{
					if (!isRestrictedTopic || isUserOp)
					{
						if (getInput()[2] == "::")
						{
							it->second.setTopic("");
							broadcastTOPIC(client, it->first);
						}
						else
						{
							it->second.setTopic(getInput()[2]);
							broadcastTOPIC(client, it->first);
						}
					}
					else
						SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, getInput()[1]), "Error while sending TOPIC message");
				}
			}
		}
		return;
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
				SEND(user_it->second.getSocket(), RPL_PRIVMSG(channelName, getMsgToSend(message)), "Error sending message to channel.");
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
				if (*mode_it == 'i' || *mode_it == 'k')
				{
					if (isUserInvited(client.getNick(), channelName))
					{
						if (!getPassword().empty()) // requer password para acessar
						{
							if (input[2].empty() || input[2] != getPassword())
							{
								SEND(client.getSocket(), ERR_PASSWDMISMATCH(client), "Error sending JOIN message with mode +k advise to user");
								return;
							}
						}
						if (!Utils::isOperator(it->second, client.getNick())) // operadores nunca saem da lista de convidados do canal
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
						SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
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
					setNewChannel(channelName, channel);
					SEND(clientSocket, RPL_JOIN(client, channelName), "Erro ao entrar no canal.");
					MODE("MODE", client);
					WHO(client.getSocket(), client);
				}
			}
			++inputIterator;
		}
		else
			++inputIterator;
	}
}

void Server::INVITE(Client client)
{
	bool userAvailable = false;
	bool channelAvailable = false;
	int invitedFd;
	std::string channelName;

	if (getInput().size() != 3)
		return;

	if (getInput()[2].size() > 1)
		channelName = getInput()[2].substr(1);
	else
		return;
	const std::map<std::string, Channel>::iterator it = getChannels().find(channelName);

	if (getInput()[1] == client.getNick())
	{
		SEND(client.getSocket(), ERR_USERONCHANNEL(client.getNick(), client.getNick(), channelName), "Error sending ERR_CHANOPRIVSNEEDED (482)");
		return;
	}

	try
	{
		if (!Utils::isOperator(it->second, client.getNick()))
		{
			SEND(client.getSocket(), ERR_CHANOPRIVSNEEDED(client, getInput()[2]), "Error sending ERR_CHANOPRIVSNEEDED (482)");
			return;
		}
	}
	catch (std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}

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
			channelAvailable = true;
			std::map<std::string, Client> &users = this->getGlobalUsers();
			std::map<std::string, Client>::iterator us = users.begin();
			while (us != users.end())
			{
				if (us->first == invitedUser)
				{
					userAvailable = true;
					invitedFd = us->second.getSocket();
					break;
				}
				++us;
			}
			break;
		}
		++ch;
	}

	if (!channelAvailable)
	{
		SEND(client.getSocket(), ERR_NOSUCHCHANNEL(client, channel), "Error sending msg ERR_NOSUCHCHANNEL");
		return;
	}

	if (userAvailable)
		ch->second.AddInvited(invitedUser);
	else
	{
		SEND(client.getSocket(), ERR_NOSUCHNICK(client, invitedUser), "Error sending msg ERR_NOSUCHNICK");
		return;
	}

	std::string msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " INVITE " + invitedUser + " " + channel + "\r\n";
	SEND(invitedFd, msg, "Error sending INVITE message");
	SEND(invitedFd, msg, "Error sending INVITE message");
}

void Server::WHO(int clientSocket, const Client client)
{
	(void)clientSocket;
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
	if (it != channels.end())
	{
		std::string listUsers;
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
			listUsers += nickname;
			if (++user_it != users.end())
				listUsers += " ";
		}
		SEND(client.getSocket(), RPL_NAMREPLY(client, channelName, listUsers), "Error sending WHO");
		SEND(client.getSocket(), RPL_ENDOFNAMES(client, channelName), "Error sending WHO");
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
				SEND(client.getSocket(), RPL_PART(channelName, getMsgToSend(message)), "Erro ao enviar mensagem de saída de canal.");
				users.erase(us);
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
					broadcastKICK(client, kickNick, channelName, reason);
					users.erase(us);
					// WHO(us->second.getSocket(), us->second);
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
