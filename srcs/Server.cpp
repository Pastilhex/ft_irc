/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:38:21 by ialves-m          #+#    #+#             */
/*   Updated: 2024/04/06 23:52:38 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ircserv.hpp"

Server::Server(std::string password)
{
	this->_password = password;
}

void ft_print(std::string str)
{
	std::cout << str << std::endl;
}

/**
 * @brief Obtém o descritor de arquivo do socket do servidor.
 *
 * Esta função retorna o descritor de arquivo do socket do servidor.
 *
 * @return Retorna o descritor de arquivo do socket do servidor.
 */
int Server::getSocket(void)
{
	return this->_socket;
}

std::string Server::getHostname(void) const
{
	return this->_hostname;
}

/**
 * @brief Obtém a estrutura sockaddr_in do servidor.
 *
 * Esta função retorna a estrutura sockaddr_in que representa o endereço do servidor.
 *
 * @return Retorna a estrutura sockaddr_in do servidor.
 */
struct sockaddr_in Server::getAddress(void)
{
	return this->_address;
}

std::string Server::getPassword(void)
{
	return this->_password;
}

std::map<std::string, Channel> &Server::getChannels(void)
{
	return this->_channels;
}

void Server::setChannel(std::string channel_name, bool state)
{
	_channels.insert(std::make_pair(channel_name, Channel(channel_name, state)));
}

/**
 * @brief Define o descritor de arquivo do socket do servidor.
 *
 * Esta função define o descritor de arquivo do socket do servidor para um novo valor.
 *
 * @param newSocket O novo valor do descritor de arquivo do socket do servidor.
 */
void Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

void Server::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

/**
 * @brief Define a estrutura sockaddr_in do servidor.
 *
 * Esta função define a estrutura sockaddr_in que representa o endereço do servidor para um novo valor.
 *
 * @param newAddress O novo valor da estrutura sockaddr_in do servidor.
 */
void Server::setAddress(struct sockaddr_in newAddress)
{
	this->_address = newAddress;
}

/**
 * @brief Verifica se uma porta é válida.
 *
 * Esta função verifica se uma  std::string representa uma porta válida.
 *
 * @param str A  std::string contendo a porta a ser verificada.
 * @return Retorna true se a porta for válida, false caso contrário.
 */
bool Server::isValidPort(char *str)
{
	for (size_t i = 0; i < strlen(str); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}

/**
 * @brief Função para obter o socket do servidor.
 *
 * Esta função cria e configura um socket do servidor.
 *
 * @return Retorna o descritor de arquivo do socket do servidor se bem-sucedido,
 *         -1 se ocorrer um erro ao criar o socket,
 *         1 se ocorrer um erro ao definir o modo não-bloqueante para o socket.
 */
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

/**
 * @brief Cria uma estrutura sockaddr_in para o servidor.
 *
 * Esta função cria e configura uma estrutura sockaddr_in para representar o endereço do servidor.
 *
 * @param port A porta do servidor.
 * @return Retorna a estrutura sockaddr_in configurada.
 */
struct sockaddr_in Server::createAddress(int port)
{
	struct sockaddr_in serverAddress;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	return serverAddress;
}

/**
 * @brief Obtém informações de endereço do servidor.
 *
 * Esta função obtém e exibe o endereço IP local do servidor.
 */
std::string Server::getAddressIP(void)
{
	// char hostname[256];
	// if (gethostname(hostname, sizeof(hostname)) == -1)
	// {
	// 	std::cerr << "Erro ao obter o nome do host." << std::endl;
	// }

	// struct hostent *host_info = gethostbyname(hostname);
	// if (host_info == NULL || host_info->h_addr_list[0] == NULL)
	// {
	// 	std::cerr << "Erro ao obter o endereço IP." << std::endl;
	// }

	// char *ip_address = inet_ntoa(*((struct in_addr *)host_info->h_addr_list[0]));

	char hostname[256];
	char *ipAddress;

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

/**
 * @brief Inicia o servidor.
 *
 * Esta função inicia o servidor, configurando o socket e o endereço para comunicação.
 *
 * @param str Uma  std::string contendo a porta do servidor.
 * @return Retorna true se o servidor foi iniciado com sucesso,
 *         false caso contrário.
 */
bool Server::start(char *str)
{
	if (!isValidPort(str))
		return false;
	this->_port = atoi(str);
	this->_socket = createSocket();
	this->_address = createAddress(this->_port);
	return true;
}

/**
 * @brief Associa o socket do servidor a um endereço específico.
 *
 * Esta função associa o socket do servidor a um endereço IP e número de porta específicos.
 *
 * @param serverSocket O descritor de arquivo do socket do servidor.
 * @param serverAddress A estrutura sockaddr_in contendo o endereço IP e número de porta do servidor.
 * @return Retorna true se o socket foi associado ao endereço com sucesso,
 *         false caso contrário.
 */
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

/**
 * @brief Verifica as conexões do servidor.
 *
 * Esta função configura o socket do servidor para entrar no modo de escuta
 * para aceitar conexões de entrada dos clientes.
 *
 * @param serverSocket O descritor de arquivo do socket do servidor.
 * @return Retorna true se o socket foi colocado no modo de escuta com sucesso,
 *         false caso contrário.
 */
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

		// Verifica se é um novo client
		isNewClient(fds, serverSocket, clientAddress, client);

		// verifica dentro do vector<pollfd> qual dos clients vai tratar dos dados
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				client = getClientBySocket(fds[i].fd, client);
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
					processMsg(client, fds, buffer, bytesRead, i);
				}
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
	std::cout << "<<:\n" << std::string(buffer, bytesRead) << std::endl;

	if (isCMD(message, "NICK") || isCMD(message, "USER ") || isCMD(message, "PASS"))
	{
		client.getClientLoginData(buffer, bytesRead);
		fds[i].revents = 0;
	}

	if (isCMD(message, "MODE"))
	{
	}

	if (isCMD(message, "WHO"))
	{
		std::string channelName = getInput(message, "WHO ");
		WHO(fds[i].fd, client, channelName);
		fds[i].revents = 0;
	}

	if (isCMD(message, "LIST"))
	{
		LIST(fds[i].fd, client, message);
		fds[i].revents = 0;
	}

	if (isCMD(message, "JOIN"))
	{
		JOIN(fds[i].fd, client, message);
		fds[i].revents = 0;
	}

	if (isCMD(message, "PART"))
	{
		// PART #canalX :Até logo, pessoal!
		std::string channelName = getInput(message, "PART");
		PART(client, channelName);
		fds[i].revents = 0;
	}
	if (message.find("QUIT") != std::string::npos)
	{
		close(fds[i].fd); // Fecha a conexão com o cliente
		// Envie uma mensagem de saída para todos os usuarios dos canais que o cliente estava
		// Remova o cliente de todos os canais
		fds.erase(fds.begin() + i); // Remova o cliente do vector<pollfd>
	}
	if (message.find("MODE") != std::string::npos)
	{
		MODE(message, client);
		fds[i].revents = 0;
	}
}

void Server::Send_PRIVMSG_toChannel(Client client, std::string channelName)
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
			WHO(user_it->second.getSocket(), user_it->second, channelName);
			++user_it;
		}
	}
}

void Server::Send_WHO_toAll(Client client, std::string channelName)
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
			WHO(user_it->second.getSocket(), user_it->second, channelName);
			++user_it;
		}
		return;
	}
}

void Server::LIST(int clientSocket, Client &client, std::string message)
{
	(void)message;
	(void)client;
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		std::string channel_name = it->first;
		size_t pos = channel_name.find('\n');
		if (pos != std::string::npos)
		{
			channel_name.erase(pos, 1);
		}
		int nbrUser = it->second.getNbrUsers();
		char nbrUserStr[20];				// Tamanho suficiente para armazenar um inteiro
		sprintf(nbrUserStr, "%d", nbrUser); // Formatar o inteiro como uma string
		std::string channel = ":" + this->getHostname() + " 322 " + client.getNick() + " #" + channel_name + " " + std::string(nbrUserStr) + " :" + it->second.getTopic() + "\r\n";
		channel += ":" + this->getHostname() + " 323 " + client.getNick() + " :End of /LIST\r\n";
		send(clientSocket, channel.c_str(), channel.size(), 0);
		std::cout << ">> " + channel << std::endl;
	}
}

void Server::JOIN(int clientSocket, Client &client, std::string message)
{
	size_t posCmd = message.find("JOIN");
	size_t posCmdLower = message.find("join");
	if (posCmd != std::string::npos || posCmdLower != std::string::npos)
	{
		size_t posChannel = message.find_first_not_of(" \n\r\t", posCmd + 4);
		std::string channelName = message.substr(posChannel + 1, message.find_first_of(" \n\r\t", posChannel + 1) - (posChannel + 1));

		if (!(message[posChannel] == '#' || (message[posChannel] == '&')))
			std::cout << ("Not a valid channel name, try with '#' or '&'") << std::endl;

		std::map<std::string, Channel> &channels = getChannels(); // Criar uma ligação com a lista de canais
		std::map<std::string, Channel>::iterator it = channels.find(channelName);
		if (message[posChannel] == '#' || message[posChannel] == '&')
		{
			for (it = channels.begin(); it != channels.end(); ++it)
			{
				if (it->first == channelName)
				{
					it->second.setNewUser(client);
					Send_WHO_toAll(client, channelName);
					break;
				}
			}

			if (it == channels.end())
			{
				bool state = (message[posChannel] == '#') ? false : true;
				Channel channel = Channel(channelName, state);
				channel.setNewUser(client);
				channel.AddOperator(client.getNick());
				_channels.insert(std::make_pair(channelName, channel)); // Fazer um setter para esta função
				// removeClientFromGlobalUsers(client);
			}
			else
			{
				// std::map<std::string, Channel>::iterator in = channels.begin();
				// removeClientFromGlobalUsers(client);
			}
		}

		std::map<std::string, Channel>::iterator newIt = channels.find(channelName);
		if (newIt != channels.end())
		{
			Channel &channel = newIt->second;
			std::string topic = channel.getTopic();
			std::string joinMsg = ":" + client.getNick() + " JOIN " + message[posChannel] + channelName + " :" + topic + "\r\n";
			std::cout << joinMsg << std::endl;
			if (send(clientSocket, joinMsg.c_str(), joinMsg.length(), 0) == -1)
			{
				std::cerr << "Erro ao entrar no canal." << std::endl;
			}
		}
		else
		{
			std::cout << "Canal não encontrado" << std::endl;
		}
	}
}

void Server::WHO(int clientSocket, const Client client, std::string channelName)
{
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(channelName);

	bool channelPrivacy = it->second.getModePrivateAccess();
	std::string privacy = (channelPrivacy) ? "@" : "#";

	if (it != channels.end())
	{
		std::string whoMsg = ":" + getHostname() + " 353 " + client.getNick() + " = " + privacy + channelName + " :";
		const std::map<std::string, Client> &users = it->second.getUsers();
		std::map<std::string, Client>::const_iterator user_it = users.begin();
		while (user_it != users.end())
		{
			std::string nickname = user_it->first;
			std::vector<std::string> opList = it->second.getOperators();
			std::vector<std::string>::iterator op_it = opList.begin();
			while (op_it != opList.end())
			{

				if ((*op_it).find(nickname, 1) != std::string::npos) // esta condição não está a funcionar bem
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
		whoMsg += "\r\n:" + getHostname() + " 366 " + client.getNick() + " " + privacy + channelName + " :End of Names list.\r\n";
		std::cout << whoMsg << std::endl;
		if (send(clientSocket, whoMsg.c_str(), whoMsg.length(), 0) == -1)
		{
			std::cerr << "Erro ao enviar mensagem de boas vindas para o cliente." << std::endl;
		}
	}
}

void Server::PART(Client &client, std::string channelName)
{
	int isInChannels = 0;
	std::map<std::string, Channel> &channels = getChannels();
	std::map<std::string, Channel>::iterator channels_begin = channels.begin();
	std::map<std::string, Channel>::iterator channels_end = channels.end();

	for (std::map<std::string, Channel>::iterator &it = channels_begin; it != channels_end; ++it)
	{
		std::map<std::string, Client> &users = it->second.getUsers();
		const std::map<std::string, Client>::iterator &users_it = users.begin();
		while (users_it != users.end())
		{
			if (users_it->second.getNick() == client.getNick())
			{
				isInChannels++;
				break;
			}
		}
	}
	if (isInChannels == 1)
	{
		std::map<std::string, Channel> &channels = getChannels();
		std::map<std::string, Channel>::iterator channels_begin = channels.begin();
		std::map<std::string, Channel>::iterator channels_end = channels.end();

		for (std::map<std::string, Channel>::iterator &it = channels_begin; it != channels_end; ++it)
		{
			std::map<std::string, Client> &users = it->second.getUsers();
			const std::map<std::string, Client>::iterator &users_it = users.begin();
			while (users_it != users.end())
			{
				if (users_it->second.getNick() == client.getNick())
				{
					// Adiciona o client à lista global de usuarios
					
					users.erase(users_it);
					std::string leaveChannel = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressIP() + " PART #" + channelName + "\r\n";
					std::cout << leaveChannel << std::endl;
					if (send(client.getSocket(), leaveChannel.c_str(), leaveChannel.length(), 0) == -1)
					{
						std::cerr << "Erro ao enviar mensagem de saida de canal." << std::endl;
					}
					break;
				}
			}
		}
	}
}

/**
 * @brief Executa o servidor.
 *
 * Esta função inicia o servidor e executa as etapas necessárias para aceitar conexões dos clientes.
 *
 * @return Retorna true se o servidor foi iniciado e as conexões foram verificadas com sucesso,
 *         false caso contrário.
 */
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

/**
 * @brief Envia uma mensagem de boas-vindas para o cliente.
 *
 * Esta função envia uma mensagem de boas-vindas para o cliente conectado ao servidor.
 * A mensagem contém informações sobre o servidor e uma saudação ao cliente.
 *
 * @param clientSocket O descritor de arquivo do socket do cliente.
 */
void Server::sendWelcome(int clientSocket, Client &client)
{
	std::string welcome = ":localhost 001 pastilhex :Welcome to the Internet Relay Network, " + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + "!" + getAddressIP() + "\r\n";
	welcome += ":localhost 002 pastilhex :Your host is " + getHostname() + ", running version FT_IRC_42Porto_v1.0\r\n";
	welcome += ":localhost 003 pastilhex :This server was created " + getCurrentDateTime() + "\r\n";
	welcome += ":localhost 372 pastilhex :███████╗████████╗    ██╗██████╗  ██████╗\r\n";
	welcome += ":localhost 372 pastilhex :██╔════╝╚══██╔══╝    ██║██╔══██╗██╔════╝\r\n";
	welcome += ":localhost 372 pastilhex :█████╗     ██║       ██║██████╔╝██║     \r\n";
	welcome += ":localhost 372 pastilhex :██╔══╝     ██║       ██║██╔══██╗██║     \r\n";
	welcome += ":localhost 372 pastilhex :██║        ██║ ████╗ ██║██║  ██║╚██████╗\r\n";
	welcome += ":localhost 372 pastilhex :╚═╝        ╚═╝  ╚══╝ ╚═╝╚═╝  ╚═╝ ╚═════╝\r\n";
	welcome += ":localhost 372 pastilhex :Project by:  ialves-m  lpicoli  jhogonca\r\n";
	welcome += ":localhost 376 pastilhex :End of /MOTD command.\r\n";

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