#ifndef _BOT_
#define _BOT_

#include "ircserv.hpp"

class Bot
{
	public:

		static void create(Server &server, Channel &channel, std::string bot_nick, Client &client);
		static void remove(Server &server, Channel &channel, Client &client);
		static void help();
		static void sendWelcome(Server server, Channel &channel, Client &client);
		static void sendGoodbye(Server server, Channel &channel, Client &client);
		virtual void execute() = 0; 
		
};

// class Bot : public Client
// {
// private:
// 	std::vector<std::string> _commands;
// public:
// 	Server server;
// 	// Constructor
// 	Bot(std::string nick, Server &server);
// 	//~Bot(){};
// 	//void welcomeNewClient(Client client);
// };

#endif