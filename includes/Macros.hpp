/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:22:32 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/08 22:22:32 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MACROS_
# define _MACROS_

#define DATE std::string(__TIME__) +" "+ std::string(__DATE__)
#define USER_ID(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define LOCAL ":localhost "

// Server messages
#define RPL_WELCOME(user_id, nickname) \
	(std::string(LOCAL) + "001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
#define RPL_YOURHOST(client, servername, version) \
	(std::string(LOCAL) + "002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
#define RPL_CREATED(client, datetime) \
	(std::string(LOCAL) + "003 " + client + " :This server was created " + datetime + "\r\n")
#define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) \
	(std::string(LOCAL) + "004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
#define RPL_ISUPPORT(client, tokens) \
	(std::string(LOCAL) + "005 " + client + " " + tokens + " :are supported by this server\r\n")

// Error messages
#define RPL_INVITING(nickname, target) \
	(std::string(LOCAL) + "341 " + nickname + " " + target + " :Inviting user to channel\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) \
	(std::string(LOCAL) + "403 " + client + " #" + channel + " :No such channel\r\n")
#define ERR_UNKNOWNCOMMAND(client, command) \
	(std::string(LOCAL) + "421 " + client + " " + command + " :Unknown command\r\n")
#define ERR_NOTONCHANNEL(client, channel) \
	(std::string(LOCAL) + "442 " + client + " #" + channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) \
	(std::string(LOCAL) + "443 " + client + " " + nick + " #" + channel + " :is already on channel\r\n")
#define ERR_NEEDMOREPARAMS(client, command) \
	(std::string(LOCAL) + "461 " + client + " " + command + " :Not enough parameters.\r\n")

// JOIN
#define RPL_JOIN(user_id, channel) \
	(user_id + " JOIN :#" + channel + "\r\n")
#define ERR_BANNEDFROMCHAN(client, channel) \
	("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(client, channel) \
	("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

// KICK
#define ERR_USERNOTINCHANNEL(client, nickname, channel) \
	("441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")
#define RPL_KICK(user_id, channel, kicked, reason) \
	(user_id + " KICK #" + channel + " " + kicked + " " + reason + "\r\n")

// KILL
#define ERR_NOPRIVILEGES(client) \
	("481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
#define RPL_KILL(user_id, killed, comment) \
	(user_id + " KILL " + killed + " " + comment + "\r\n")

// MODE
#define RPL_UMODEIS(client, mode) \
	(std::string(LOCAL) + "221 " + client + " " + mode + "\r\n")
#define MODE_USERMSG(client, mode) \
	(":" + client + " MODE " + client + " :" + mode + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) \
	(std::string(LOCAL) + "501 " + client + " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(client) \
	("502 " + client + " :Cant change mode for other users\r\n")
#define MODE_CHANNELMSG(channel, mode) \
	(std::string(LOCAL) + "MODE #" + channel + " " + mode + "\r\n")
#define MODE_CHANNELMSGWITHPARAM(channel, mode, param) \
	(std::string(LOCAL) + "MODE #" + channel + " " + mode + " " + param + "\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode) \
	(std::string(LOCAL) + "324 " + client + " #" + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) \
	(std::string(LOCAL) + "324 " + client + " #" + channel + " " + mode + " " + password + "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) \
	("404 " + client + " #" + channel + " :Cannot send to channel\r\n")
#define ERR_CHANNELISFULL(client, channel) \
	("471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) \
	(std::string(LOCAL) + "482 " + client + " #" + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) \
	("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
#define RPL_ADDVOICE(nickname, username, channel, mode, param) \
	(":" + nickname + "!" + username + "@localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

// MOTD
#define ERR_NOSUCHSERVER(client, servername) \
	(std::string(LOCAL) + "402 " + client + " " + servername + " :No such server\r\n")
#define ERR_NOMOTD(client) \
	(std::string(LOCAL) + "422 " + client + " :MOTD File is missing\r\n")
#define RPL_MOTDSTART(client, servername) \
	(std::string(LOCAL) + "375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) \
	(std::string(LOCAL) + "372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) \
	(std::string(LOCAL) + "376 " + client + " :End of /MOTD command.\r\n")

// NAMES
#define RPL_NAMREPLY(client, symbol, channel, list_of_nicks) \
	(std::string(LOCAL) + "353 " + client + " " + symbol + " #" + channel + " :" + list_of_nicks + "\r\n")
#define RPL_ENDOFNAMES(client, channel) \
	(std::string(LOCAL) + "366 " + client + " #" + channel + " :End of /NAMES list.\r\n")

// NICK
#define ERR_NONICKNAMEGIVEN(client) \
	(std::string(LOCAL) + "431 " + client + " :There is no nickname.\r\n")
#define ERR_ERRONEUSNICKNAME(client, nickname) \
	(std::string(LOCAL) + "432 " + client + " " + nickname + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(client, nickname) \
	(std::string(LOCAL) + "433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
#define RPL_NICK(oclient, uclient, client) \
	(":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

// NOTICE
#define RPL_NOTICE(nick, username, target, message) \
	(":" + nick + "!" + username + "@localhost NOTICE " + target + " " + message + "\r\n")

// OPER
#define ERR_NOOPERHOST(client) \
	("491 " + client + " :No O-lines for your host\r\n")
#define RPL_YOUREOPER(client) \
	("381 " + client + " :You are now an IRC operator\r\n")

// PART
#define RPL_PART(user_id, channel, reason) \
	(user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")

// PASS
#define ERR_PASSWDMISMATCH(client) \
	(LOCAL + "464 " + client + " :Password incorrect.\r\n")

// PING
#define RPL_PONG(user_id, token) \
	(user_id + " PONG " + token + "\r\n")

// QUIT
#define RPL_QUIT(user_id, reason) \
	(user_id + " QUIT :Quit: " + reason + "\r\n")
#define RPL_ERROR(user_id, reason) \
	(user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
#define ERR_NOSUCHNICK(client, target) \
	("401 " + client + " " + target + " :No such nick/channel\r\n")
#define ERR_NORECIPIENT(client) \
	("411 " + client + " :No recipient given PRIVMSG\r\n")
#define ERR_NOTEXTTOSEND(client) \
	("412 " + client + " :No text to send\r\n")
#define RPL_PRIVMSG(nick, username, target, message) \
	(":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
#define RPL_TOPIC(client, channel, topic) \
	(LOCAL + "332 " + client + " #" + channel + " " + topic + "\r\n")
#define RPL_NOTOPIC(client, channel) \
	(LOCAL + "331 " + client + " #" + channel + " :No topic is set\r\n")

// USER
#define ERR_ALREADYREGISTERED(client) \
	(LOCAL + "462 " + client + " :You may not reregister.\r\n")

#endif