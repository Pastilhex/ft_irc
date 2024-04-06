#ifndef _UTILS_
# define _UTILS_

#include "ircserv.hpp"


class Utils
{
    private:
        static bool hasNonDigits(const std::string &str);
        static bool isValidPort(const std::string &portStr);
        static bool isValidPassword(const std::string &str);

    public:
        static bool isValidInput(const std::string &password, const std::string &port);
        static std::string getCurrentDateTimeAsString();
        static std::string findWord(const std::string &keyword, const std::string &delimiter, const std::string &text);
        static int logMessage(const std::string &message, int level);
        static std::vector<std::string> split(const std::string& str, const std::string& delimiters);
        static bool isValidUser(Channel channel, std::string nickname);
        static bool isValidFlag(std::string flag);
        void signalHandler(int signum);
};

#endif