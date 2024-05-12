#ifndef _BOT_
#define _BOT_

#include "ircserv.hpp"

class Bot
{
public:
	static void create(Server &server, Channel &channel, std::string bot_nick, Client &client);
	static void remove(Server &server, Channel &channel, Client &client);
	static void help(Server server, Channel &channel, Client &client);
	static void sendWelcome(Server server, Channel &channel, Client &client);
	static void sendGoodbye(Server server, Channel &channel, Client &client);
	static void quote(Server server, Channel &channel, Client &client);
	static void funFact(Server server, Channel &channel, Client &client);

	virtual void execute() = 0;
};

#endif