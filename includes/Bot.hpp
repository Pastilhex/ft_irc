#ifndef _BOT_
#define _BOT_

#include "ircserv.hpp"

class Bot : public Client {

    public:
        Server server;

        Bot(std::string nick, std::string serverPassword, Server server);
        ~Bot();

        void welcomeNewClient(Client client);
        //void setServer(Server &server) const;
        //pomodoro
        //help 
};
#endif