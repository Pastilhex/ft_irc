#include "../includes/Bot.hpp"
#include "../includes/Macros.hpp"


#define BOT_NICKNAME this->getNick()

Bot::Bot(std::string nick, std::string serverPassword, Server server) : Client(){
    this->server = server;
    this->_nick = nick;
    this->_username = nick;
    this->_serverPassword = serverPassword;
}

Bot::~Bot(){}

#include <sys/socket.h>

void Bot::welcomeNewClient(Client client) {
    
    std::cout << "Welcome to the server!" << client.getNick() << std::endl;

    //std::string msg = ":" + BOT_NICKNAME + "!" + BOT_NICKNAME + "@" + server.getHostname() + " PRIVMSG " + BOT_NICKNAME + " :" + "Welcome to the server, " + client.getNick() + "!\r\n";
    //send(server.getSocket(), msg.c_str(), msg.length(), 0);
}


