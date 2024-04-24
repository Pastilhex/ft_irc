#include "../includes/Bot.hpp"
#include "../includes/Macros.hpp"
#include <sys/socket.h>


#define BOT_NICKNAME this->getNick()

Bot::Bot(std::string nick, Server &server) : Client() {
    if (nick.empty())
        this->setNick("Bot");
    else
        this->setNick(nick);
    this->server = server;
    this->_username = "Marvin";
    this->_realname = "Bot";
}

Client Bot::createBot(Server *server, std::string channel)
{
    Client *bot = new Bot("Bot", *server);
    bot->setUsername("Marvin");

    std::map<std::string, Channel>::iterator it = server->getChannels().find(channel);
    if (it == server->getChannels().end())
        return 0;
    std::map<std::string, Client> &users = it->second.getUsers();
    std::map<std::string, Client>::iterator us = users.begin();
    while (us != users.end())
    {
        if (us->second.getRealName() == "Bot")
            return NULL;
        us++;
    }
    users.insert(std::pair<std::string, Client>("Bot", *bot));
    return *bot;
}

void Bot::welcomeNewClient(Client client) {
    
    std::cout << "Welcome to the server!" << client.getNick() << std::endl;

    //std::string msg = ":" + BOT_NICKNAME + "!" + BOT_NICKNAME + "@" + server.getHostname() + " PRIVMSG " + BOT_NICKNAME + " :" + "Welcome to the server, " + client.getNick() + "!\r\n";
    //send(server.getSocket(), msg.c_str(), msg.length(), 0);
}


