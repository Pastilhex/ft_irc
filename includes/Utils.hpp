#ifndef _UTILS_
# define _UTILS_

#include "ircserv.hpp"


class Utils
{
    private:
        /**
         * Checks if a given string contains non-digit characters.
         *
         * @param str The string to check.
         * @return True if the string contains non-digit characters, false otherwise.
         */
        static bool hasNonDigits(const std::string &str);

        /**
         * Checks if a given string is a valid port number.
         *
         * @param portStr The string to check.
         * @return True if the string is a valid port number, false otherwise.
         */
        static bool isValidPort(const std::string &portStr);

        /**
         * Checks if a given string is a valid password.
         *
         * @param str The string to check.
         * @return True if the string is a valid password, false otherwise.
         */
        static bool isValidPassword(const std::string &str);

    public:

        /**
         * @brief Checks if the input is valid.
         *
         * This function checks if the given password and port are valid inputs.
         *
         * @param password The password to be checked.
         * @param port The port to be checked.
         * @return true if the input is valid, false otherwise.
         */
        static bool isValidInput(const std::string &password, const std::string &port);

        /**
         * @brief Get the current date and time as a string.
         *
         * @return The current date and time as a string.
         */
        static std::string getCurrentDateTimeAsString();

        /**
         * @brief Finds the first occurrence of a keyword in a given text, delimited by a specified delimiter.
         * 
         * @param keyword The keyword to search for.
         * @param delimiter The delimiter used to separate words in the text.
         * @param text The text to search within.
         * @return The first occurrence of the keyword found in the text, or an empty string if not found.
         */
        static std::string findWord(const std::string &keyword, const std::string &delimiter, const std::string &text);

        /**
         * @brief Logs a message to the console.
         *
         * @param message The message to log.
         * @param level The level of the message.
         * @return The level of the message.
         */
        static int logMessage(const std::string &message, int level);

        /**
         * @brief Splits a string into a vector of strings based on a delimiter.
         *
         * @param str The string to split.
         * @param delimiters The delimiters to split the string by.
         * @return A vector of strings containing the split string.
         */
        static std::vector<std::string> split(const std::string& str, const std::string& delimiters);

        /**
         * @brief Checks if a given string is a valid user.
         *
         * This function checks if the given nickname is a valid user in the given channel.
         *
         * @param channel The channel to check.
         * @param nickname The nickname to check.
         * @return true if the user is valid, false otherwise.
         */
        static bool isValidUser(Channel channel, std::string nickname);

        /**
         * @brief Checks if a given string is a valid mode's flag.
         *
         * This function checks if the given flag is a valid mode's flag.
         *
         * @param flag The flag to check.
         * @return true if the flag is valid, false otherwise.
         */
        static bool isValidFlag(char flag);


        /**
         * @brief Checks if a given nickname is an operator in a specific channel.
         * 
         * @param channel The channel to check for operator status.
         * @param nickname The nickname to check.
         * @return True if the nickname is an operator in the channel, false otherwise.
         */
        static bool isOperator(Channel channel, std::string nickname);


        /**
         * @brief Handles the specified signal.
         *
         * This function is responsible for handling the specified signal.
         *
         * @param signum The signal number to handle.
         */
        void signalHandler(int signum);

        static std::string getChannelByChannelName(const std::string& channel);
};

#endif