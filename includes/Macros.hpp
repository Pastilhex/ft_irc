/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:22:32 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/23 12:35:46 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MACROS_
# define _MACROS_

/* Constructs a date string in the format "HH:MM:SS Mon DD YYYY"
 * Example: 16:20:32 Apr  17 2024 */
#define DATE std::string(__TIME__) +" "+ std::string(__DATE__)

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

#define RPL_ISUPPORT(client, tokens) \
	(std::string(HOST) + " 005 " + client + " " + tokens + " :are supported by this server\r\n")

// Error messages

// Invites a user to a channel
// Example: :localhost 341 nick target :Inviting user to channel
#define RPL_INVITING(nickname, target) \
	(std::string(HOST) + " 341 " + nickname + " " + target + " :Inviting user to channel\r\n")

// Indicates that the specified channel does not exist
// Example: :localhost 403 user123 #channel :No such channel
#define ERR_NOSUCHCHANNEL(client, channel) \
	(":" + getHostname() + " 403 " + client.getNick() + " " + channel + " :No such channel\r\n")

// Indicates that the specified command is unknown
// Example: :localhost 421 user123 command :Unknown command
#define ERR_UNKNOWNCOMMAND(client, command) \
	(std::string(HOST) + " 421 " + client + " " + command + " :Unknown command\r\n")

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

// Indicates that a client cannot join a channel because they are banned
// Example: 474 user123 #channel :Cannot join channel (+b)
#define ERR_BANNEDFROMCHAN(client, channel) \
	("474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")

// Indicates that a client cannot join a channel because they provided a bad channel key
// Example: 475 user123 #channel :Cannot join channel (+k)
#define ERR_BADCHANNELKEY(client, channel) \
	("475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

// KICK

// Indicates that a user cannot be kicked because they are not in the specified channel
// Example: 441 user123 nick #channel :They aren't on that channel
#define ERR_USERNOTINCHANNEL(client, nickname, channel) \
	(" 441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")

// Indicates that a user has been kicked from a channel
// Example: :user123!username@localhost KICK #channel kickedUser :reason
#define RPL_KICK(client, channel, kicked, reason) \
	(":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " KICK " + channel + " " + kicked + " :" + reason + "\r\n")
// KILL
#define ERR_NOPRIVILEGES(client) \
	(std::string(HOST) + " 481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
#define RPL_KILL(user_id, killed, comment) \
	(user_id + " KILL " + killed + " " + comment + "\r\n")

// MODE

/* :localhost 221 user123 +i */
#define RPL_UMODEIS(client, mode) \
	(std::string(HOST) + " 221 " + client + " " + mode + "\r\n")

/* :user123 MODE user123 :+i */
#define MODE_USERMSG(client, mode) \
	(":" + client + " MODE " + client + " :" + mode + "\r\n")

/* :localhost 501 user123 :Unknown MODE flag */
#define ERR_UMODEUNKNOWNFLAG(client) \
	(std::string(HOST) + " 501 " + client + " :Unknown MODE flag\r\n")

/* :localhost 502 user123 :Cant change mode for other users */
#define ERR_USERSDONTMATCH(client) \
	(std::string(HOST) + " 502 " + client + " :Cant change mode for other users\r\n")

/* :localhost MODE #channel1 +m */
#define MODE_CHANNELMSG(channel, mode) \
	(std::string(HOST) + " MODE " + channel + " " + mode + "\r\n")

/* :localhost MODE #channel1 +l 10 */
#define MODE_CHANNELMSGWITHPARAM(client, channel, mode, param) \
	(std::string(HOST) + " 324 " + client + " " + channel + " " + mode + " " + param + "\r\n")

/* :localhost 324 user123 #channel1 +m */
#define RPL_CHANNELMODEIS(client, channel, mode) \
	(std::string(HOST) + " 324 " + client + " " + channel + " " + mode + "\r\n")

/* :localhost 324 user123 #channel1 +k password */
#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) \
	(std::string(HOST) + " 324 " + client + " #" + channel + " " + mode + " " + password + "\r\n")

/* :localhost 404 user123 #channel1 :Cannot send to channel */
#define ERR_CANNOTSENDTOCHAN(client, channel) \
	(std::string(HOST) + " 404 " + client + " #" + channel + " :Cannot send to channel\r\n")

/* :localhost 471 user123 channel1 :Cannot join channel (+l) */
#define ERR_CHANNELISFULL(client, channel) \
	(std::string(HOST) + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")

#define ERR_CHANOPRIVSNEEDED(client, channel) \
	(std::string(HOST) + " 482 " + client.getNick() + " " + channel + " :You're not channel operator\r\n")

/* :localhost 482 user123 channel1 :You're not channel operator */
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) \
	(std::string(HOST) + " 696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")

/* :localhost 696 user123 #channel1 +k password : password must only contained alphabetic character */
#define RPL_ADDVOICE(nickname, username, channel, mode, param) \
	(":" + nickname + "!" + username + "@localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

// MOTD
/* :localhost 402 user123 servername :No such server */
#define ERR_NOSUCHSERVER(client, servername) \
	(std::string(HOST) + " 402 " + client + " " + servername + " :No such server\r\n")

/* :localhost 422 user123 :MOTD File is missing */
#define ERR_NOMOTD(client) \
	(std::string(HOST) + " 422 " + client + " :MOTD File is missing\r\n")

/* :localhost 375 user123 :- servername Message of the day - */
#define RPL_MOTDSTART(client, servername) \
	(std::string(HOST) + " 375 " + client + " :- " + servername + " Message of the day - \r\n")

/* :localhost 376 user123 :End of /MOTD command. */
#define RPL_ENDOFMOTD() \
	(std::string(HOST) + " 376 " + NICKNAME + " :End of /MOTD command.\r\n")

/* :localhost 353 user123 = channel :list_of_nicks */
#define RPL_NAMREPLY(client, channel, list_of_nicks) \
	(std::string(HOST) + " 353 " + client.getNick() + " = " + channel + " :" + list_of_nicks + "\r\n")

/* :localhost 366 user123 #channel :End of /NAMES list. */
#define RPL_ENDOFNAMES(client, channel) \
	(std::string(HOST) + " 366 " + client.getNick() + " " + channel + " :End of /NAMES list.\r\n")

//LIST
/* :localhost 322 user123 #channel 5 :topic */
#define RPL_LIST(client, nbrUserStr, it) \
	(":" + this->getHostname() + " 322 " + client.getNick() + " " + channelName + " " + std::string(nbrUserStr) + " :" + it->second.getTopic() + "\r\n")

/* :localhost 323 user123 :End of /LIST */
#define RPL_LISTEND(client) \
	(":" + this->getHostname() + " 323 " + client.getNick() + " :End of /LIST\r\n")

// NICK
/* :localhost 431 user123 :There is no nickname. */
#define ERR_NONICKNAMEGIVEN(client) \
	(std::string(HOST) + " 431 " + client + " :There is no nickname.\r\n")

/* :localhost 432 user123 bad_nick :Erroneous nickname */
#define ERR_ERRONEUSNICKNAME(client, nickname) \
	(std::string(HOST) + " 432 " + client + " " + nickname + " :Erroneous nickname\r\n")

/* :localhost 433 bad_nick :Nickname is already in use. */
#define ERR_NICKNAMEINUSE(hostname, nickname) \
	(":" + hostname + " 433 " + nickname + " :Nickname is already in use.\r\n")

/* :old_nick!username@localhost NICK new_nick */
#define RPL_NICK(oclient, uclient, client) \
	(":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

// NOTICE

/* :nick!username@localhost NOTICE target :message */
#define RPL_NOTICE(nick, username, target, message) \
	(":" + nick + "!" + username + "@localhost NOTICE " + target + " :" + message + "\r\n")

// OPER

/* :491 user123 :No O-lines for your host */
#define ERR_NOOPERHOST(client) \
	(" 491 " + client + " :No O-lines for your host\r\n")

/* :381 user123 :You are now an IRC operator */
#define RPL_YOUREOPER(client) \
	(" 381 " + client + " :You are now an IRC operator\r\n")

// PART

/* :nick!username@localhost PART #channel :reason */
// Se a razão estiver vazia, a mensagem será: :nick!username@localhost PART #channel :Without any reason
#define RPL_PART(channel, reason) \
	(USER_ID(NICKNAME, USERNAME) + " PART " + channel + " " + (reason.empty() ? "Without any reason" : reason) + "\r\n")

// PASS
/* :localhost 464 user123 :Server password incorrect. \r\n */
#define ERR_PASSWDMISMATCH(client) \
	(std::string(HOST) + " 464 " + client.getNick() + " :Channel password incorrect.\r\n")

// PING :pastilhex!ivo@localhost PONG :LAG1713199744574
#define RPL_PONG(nickname, username, token) (USER_ID(nickname, username) + " PONG :" + token + "\r\n")

// QUIT

/* nick QUIT :reason */
#define RPL_QUIT(nickname, username, reason) \
	(USER_ID(nickname, username) + " QUIT :" + reason + "\r\n")

/* user_id ERROR :reason */
#define RPL_ERROR(user_id, reason) \
	(user_id + " ERROR :" + reason + "\r\n")
	
// PRIVMSG

/* :localhost 401 user123 target :No such nick/channel */
#define ERR_NOSUCHNICK(client, target) \
	(":" + getHostname() + " 401 " + client.getNick() + " " + target + " :No such nick/channel\r\n")

/* :411 user123 :No recipient given PRIVMSG */
#define ERR_NORECIPIENT(client) \
	(" 411 " + client + " :No recipient given PRIVMSG\r\n")

/* :412 user123 :No text to send */
#define ERR_NOTEXTTOSEND(client) \
	(" 412 " + client + " :No text to send\r\n")

/* :nick!username@localhost PRIVMSG #channel :message */
// Se o canal estiver vazio, a mensagem será: :nick!username@localhost PRIVMSG target :message
#define RPL_PRIVMSG(channel, message) \
	(USER_ID(NICKNAME, USERNAME) + " PRIVMSG " + ((channel == "") ? input[1] : channel) + " :" + message + "\r\n")

//:receiver!user@host PRIVMSG sender :DCC ACCEPT microshell.c 192.168.1.100 12345
#define RPL_DCC_ACCEPT(sender, file, ip, port) \
	(std::string(HOST) + " PRIVMSG " + sender + " :DCC ACCEPT " + file + " " + ip + " " + port + "\r\n")




// TOPIC

/* :localhost 332 user123 #channel :topic */
#define RPL_TOPIC(client, channel) \
	(std::string(HOST) + " 332 " + client.getNick() + " " + channel.getName() + " :" + channel.getTopic() + "\r\n")

/* :localhost 331 user123 #channel :No topic is set */
#define RPL_NOTOPIC(client, channel) \
	(std::string(HOST) + " 331 " + client.getNick() + " " + channel.getName() + " :No topic is set\r\n")

// USER

/* :localhost 462 user123 :You may not reregister. */
#define ERR_ALREADYREGISTERED(client) \
	(std::string(HOST) + " 462 " + client + " :You may not reregister.\r\n")

#endif