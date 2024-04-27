#ifndef _BOT_
#define _BOT_

#include "ircserv.hpp"

class Bot
{
	public:

		static void create(Channel &channel, std::string bot_nick);
		static void remove(Channel &channel);
		static void help();
 
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