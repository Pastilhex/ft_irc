/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:22:32 by jhogonca          #+#    #+#             */
/*   Updated: 2024/05/07 11:46:08 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MACROS_
# define _MACROS_

/* Constructs a user ID string in the format ":nickname!username@hostname"
 * Example: :nick!user@localhost */
#define USER_ID(nickname, username) \
	(":" + nickname + "!" + username + "@" + getHostname())

/* Constructs a host string in the format ":hostname"
 * Example: :localhost */
#define HOST ":" + getHostname()

/* Represents an unlimited number of users in a channel */
#define UNLIMITED_USERS 9999

/* Retrieves the nickname of the client
 * Example: nick */
#define NICKNAME client.getNick()

/* Retrieves the username of the client
 * Example: user */
#define USERNAME client.getUsername()

/* Retrieves the IP address of the client
 * Example: 192.168.1.1 */
#define IP_ADDRESS getAddressIP()

// Server messages
#define RPL_WELCOME() \
	(std::string(HOST) + " 001 " + NICKNAME + " :Welcome to the Internet Relay Network, " + NICKNAME + "!" + USERNAME + "@" + HOST + "!" + IP_ADDRESS + "\r\n")

#define RPL_YOURHOST() \
	(std::string(HOST) + " 002 " + NICKNAME + " :Your host is " + HOST + ", running version FT_IRC_42Porto_v1.0\r\n")

#define RPL_CREATED() \
	(std::string(HOST) + " 003 " + NICKNAME + " :This server was created " + getCurrentDateTime() + "\r\n")

#define RPL_MOTD(msg) \
	(std::string(HOST) + " 372 " + NICKNAME + msg)

#define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) \
	(std::string(HOST) + " 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")

// Error messages

// Indicates that the specified channel does not exist
// Example: :localhost 403 user123 #channel :No such channel
#define ERR_NOSUCHCHANNEL(client, channel) \
	(":" + getHostname() + " 403 " + client.getNick() + " " + channel + " :No such channel\r\n")

// Indicates that the client is not on the specified channel
// Example: :localhost 442 user123 #channel :You're not on that channel
#define ERR_NOTONCHANNEL(client, channel) \
	(std::string(HOST) + " 442 " + client + " " + channel + " :You're not on that channel\r\n")

// Indicates that the user is already on the specified channel
// Example: :localhost 443 user123 nick #channel :is already on channel
#define ERR_USERONCHANNEL(client, nick, channel) \
	(std::string(HOST) + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n")

// Indicates that not enough parameters were provided for a command
// Example: :localhost 461 user123 command :Not enough parameters.
#define ERR_NEEDMOREPARAMS(client, command) \
	(std::string(HOST) + " 461 " + client.getNick() + " " + command + " :Not enough parameters.\r\n")

// JOIN

// Indicates that a client has joined a channel
// Example: :user123 JOIN #channel
#define RPL_JOIN(client, channelName) \
	(":" + client.getNick() + " JOIN " + channelName + "\r\n")

// Indicates that a client cannot join a channel because it's invite-only
// Example: :localhost 473 user123 #channel :Cannot join channel (+i)
#define ERR_INVITEONLYCHAN(client, channel) \
	(":" + getHostname() + " 473 " + client.getNick() + " " + channel + " :Cannot join channel (+i)\r\n")

// Indicates that a user has been kicked from a channel
// Example: :user123!username@localhost KICK #channel kickedUser :reason
#define RPL_KICK(client, channel, kicked, reason) \
	(":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " KICK " + channel + " " + kicked + " :" + reason + "\r\n")

/* :localhost MODE #channel1 +m */
#define MODE_CHANNELMSG(channel, mode) \
	(std::string(HOST) + " MODE " + channel + " " + mode + "\r\n")

/* :localhost MODE #channel1 +l 10 */
#define MODE_CHANNELMSGWITHPARAM(client, channel, mode, param) \
	(std::string(HOST) + " 324 " + client + " " + channel + " " + mode + " " + param + "\r\n")

/* :localhost 471 user123 channel1 :Cannot join channel (+l) */
#define ERR_CHANNELISFULL(client, channel) \
	(std::string(HOST) + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

#define ERR_CHANOPRIVSNEEDED(client, channel) \
	(std::string(HOST) + " 482 " + client.getNick() + " " + channel + " :You're not channel operator\r\n")

/* :localhost 376 user123 :End of /MOTD command. */
#define RPL_ENDOFMOTD() \
	(std::string(HOST) + " 376 " + NICKNAME + " :End of /MOTD command.\r\n")

/* :localhost 353 user123 = channel :list_of_nicks */
#define RPL_NAMREPLY(client, channel, list_of_nicks) \
	(std::string(HOST) + " 353 " + client.getNick() + " = " + channel + " :" + list_of_nicks + "\r\n")

/* :localhost 366 user123 #channel :End of /NAMES list. */
#define RPL_ENDOFNAMES(client, channel) \
	(std::string(HOST) + " 366 " + client.getNick() + " " + channel + " :End of /NAMES list.\r\n")

// LIST
/* :localhost 322 user123 #channel 5 :topic */
#define RPL_LIST(client, nbrUserStr, it) \
	(":" + this->getHostname() + " 322 " + client + " " + channelName + " " + std::string(nbrUserStr) + " :" + it->second.getTopic() + "\r\n")

/* :localhost 323 user123 :End of /LIST */
#define RPL_LISTEND(client) \
	(":" + this->getHostname() + " 323 " + client.getNick() + " :End of /LIST\r\n")

// NICK
/* :localhost 431 user123 :There is no nickname. */
#define ERR_NONICKNAMEGIVEN(client, server) \
	(server.getHostname() + " 431 " + client + " :There is no nickname.\r\n")

/* :localhost 433 bad_nick :Nickname is already in use. */
#define ERR_NICKNAMEINUSE(server, nickname) \
	(":" + server.getHostname() + " 433 " + nickname + " :Nickname is already in use.\r\n")

#define RPL_PART(channel, reason) \
	(USER_ID(NICKNAME, USERNAME) + " PART " + channel + " " + (reason.empty() ? "Without any reason" : reason) + "\r\n")

// PASS
/* :localhost 464 user123 :Server password incorrect. \r\n */
#define ERR_PASSWDMISMATCH(client) \
	(std::string(HOST) + " 464 " + client.getNick() + " :Channel password incorrect.\r\n")

// PING :pastilhex!ivo@localhost PONG :LAG1713199744574
#define RPL_PONG(nickname, username, token) (":" + getHostname() + " PONG " + getHostname() + " :" + token + "\r\n")

// QUIT

/* nick QUIT :reason */
#define RPL_QUIT(nickname, username, reason) \
	(USER_ID(nickname, username) + " QUIT :" + reason + "\r\n")

/* :localhost 401 user123 target :No such nick/channel */
#define ERR_NOSUCHNICK(client, target) \
	(":" + getHostname() + " 401 " + client.getNick() + " " + target + " :No such nick/channel\r\n")

/* :nick!username@localhost PRIVMSG #channel :message */
// Se o canal estiver vazio, a mensagem será: :nick!username@localhost PRIVMSG target :message
#define RPL_PRIVMSG(channel, message) \
	(USER_ID(NICKNAME, USERNAME) + " PRIVMSG " + ((channel == "") ? input[1] : channel) + " :" + message + "\r\n")

//: receiver!user@host PRIVMSG sender :DCC ACCEPT microshell.c 192.168.1.100 12345
#define RPL_DCC_ACCEPT(sender, file, ip, port) \
	(std::string(HOST) + " PRIVMSG " + sender + " :DCC ACCEPT " + file + " " + ip + " " + port + "\r\n")

// TOPIC

/* :localhost 332 user123 #channel :topic */
#define RPL_TOPIC(client, channel) \
	(std::string(HOST) + " 332 " + client.getNick() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n")

/* :localhost 331 user123 #channel :No topic is set */
#define RPL_NOTOPIC(client, channel) \
	(std::string(HOST) + " 331 " + client.getNick() + " " + channel.getName() + " :No topic is set\r\n")

// WHOIS
// RPL_WHOISUSER (311) ":irc.server.com 311 your_nick target_nick target_ident target_host * :Real Name"
#define RPL_WHOISUSER(client, target) \
	(std::string(HOST) + " 311 " + client.getNick() + " " + target.getNick() + " " + target.getUsername() + " " + getHostname() + " * :" + client.getRealName() + "\r\n")

#endif