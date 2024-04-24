#ifndef _BOT_
#define _BOT_

#include "ircserv.hpp"

class Bot : public Client {

	public:
		Server server;

		// Constructor
		Bot(std::string nick, Server &server);
		~Bot() {};

		void welcomeNewClient(Client client);
		static Client createBot(Server *server, std::string channel);
		//void setServer(Server &server) const;
		//pomodoro
		//help 
};
#endif