/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/30 22:36:41 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Server::Server(void)
{
	this->_port = 0;
	this->_socket = 0;
	this->_hostname = "";
	this->_password = "";
	this->_quizMode = false;

	this->_botCommands.push_back("create");
	this->_botCommands.push_back("delete");
	this->_botCommands.push_back("help");
	this->_botCommands.push_back("welcome");

}

std::vector<std::string> Server::getBotCMD() const
{
	return this->_botCommands;
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

bool Server::setHostname(void)
{
	if (!createHostname())
		return false;
	return true;
}

void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

void Server::setInput(std::string message)
{
	this->_input = trimInput(message);
}

int Server::createSocket(void)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		Utils::logMessage("Erro ao criar o socket.", EXIT_FAILURE);
		return -1;
	}
	if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		Utils::logMessage("Erro ao definir modo não-bloqueante para o socket do servidor.", EXIT_FAILURE);
		close(serverSocket);
		return -1;
	}
	int optval = 1;
	if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) == -1)
	{
		Utils::logMessage("Erro ao reutilizar o socket do servidor.", EXIT_FAILURE);
		close(serverSocket);
		return -1;
	}
	return serverSocket;
}

bool Server::createHostname(void)
{
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		Utils::logMessage("Error creating hostname", 1);
		return false;
	}
	this->_hostname = hostname;
	Utils::logMessage("Hostname created successfully", EXIT_SUCCESS);
	return true;
}

struct sockaddr_in Server::createAddress(int port)
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	Utils::logMessage("Address created successfully", EXIT_SUCCESS);
	return serverAddress;
}

std::string Server::getAddressIP(void)
{
	char hostname[256];
	char *ipAddress = NULL;

	if (gethostname(hostname, sizeof(hostname)) != 0)
		std::cerr << "Erro ao obter o nome do host local" << std::endl;

	struct hostent *host = gethostbyname(hostname);
	if (host == NULL)
		std::cerr << "Erro ao obter informações do host local" << std::endl;

	struct in_addr **addr_list = reinterpret_cast<struct in_addr **>(host->h_addr_list);
	if (addr_list[0] != NULL)
		ipAddress = inet_ntoa(*addr_list[0]);
	else
		std::cerr << "Nenhum endereço IP encontrado para o host local" << std::endl;

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

bool Server::start(const std::string &port, const std::string &password)
{
	this->_password = password;
	this->_port = std::atoi(port.c_str());
	this->_address = createAddress(this->_port);

	if (!setHostname())
		return false;
	setSocket(createSocket());
	if (this->_socket == -1)
		return false;
	if (!bindSocket(this->_socket, this->_address))
		return false;
	if (!listenSocket(this->_socket))
		return false;
	Utils::logMessage("Server started successfully", EXIT_SUCCESS);
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

std::string Server::getPort(void)
{
	std::stringstream ss;
	ss << this->_port;
	return ss.str();
}

bool Server::bindSocket(const int &serverSocket, const struct sockaddr_in &serverAddress)
{
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		Utils::logMessage("Error binding socket", 1);
		close(serverSocket);
		return false;
	}
	Utils::logMessage("Socket bound successfully", EXIT_SUCCESS);
	return true;
}

bool Server::run(void)
{
	Utils::printServerInfo(getHostname(), getPort(), getAddressIP(), getPassword());
	connectClient(getSocket());
	return true;
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

std::vector<std::string> Server::trimInput(std::string msg)
{
	std::vector<std::string> words;
	std::string word;

	int begin = msg.find_first_not_of(":");
	int end = msg.find_last_not_of(" \r\n\t:,");
	std::string trimmed = msg.substr(begin, end - begin + 1);
	for (size_t i = 0; i < trimmed.size(); ++i)
	{
		if (trimmed[i] == ',')
		{
			trimmed[i] = ' ';
		}
	}

	std::stringstream ss(trimmed);
	while (ss >> word)
	{
		if (word[0] == ':')
		{
			words.push_back(word.substr(1));
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

bool Server::isBotInChannel()
{
	
}