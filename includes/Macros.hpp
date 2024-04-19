/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialves-m <ialves-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:22:32 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/19 13:27:05 by ialves-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MACROS_
# define _MACROS_

#define DATE std::string(__TIME__) +" "+ std::string(__DATE__)
#define USER_ID(nickname, username) (":" + nickname + "!" + username + "@" + getHostname())
#define HOST ":" + getHostname()
#define UNLIMITED_USERS -1
#define NICKNAME client.getNick()
#define USERNAME client.getUsername()
#define IP_ADDRESS getAddressIP()

// Server messages
#define RPL_WELCOME() \
	(std::string(HOST) + " 001 " + NICKNAME + " :Welcome to the Internet Relay Network, " + NICKNAME + "!" + USERNAME + "@" + HOST + "!" + IP_ADDRESS + "\r\n")
#define RPL_YOURHOST(client, servername, version) \
	(std::string(HOST) + " 002 " + client + " :Your host is " + servername + " (localhost), running version " + version + "\r\n")
#define RPL_CREATED(client, datetime) \
	(std::string(HOST) + " 003 " + client + " :This server was created " + datetime + "\r\n")
#define RPL_WELCOME_LOGO() \
	(std::string(HOST) + " 372 " + NICKNAME + " :███████╗████████╗    ██╗██████╗  ██████╗\r\n") \
	(std::string(HOST) + " 372 " + NICKNAME + " :██╔════╝╚══██╔══╝    ██║██╔══██╗██╔════╝\r\n") \
	(std::string(HOST) + " 372 " + NICKNAME + " :█████╗     ██║       ██║██████╔╝██║     \r\n") \
	(std::string(HOST) + " 372 " + NICKNAME + " :██╔══╝     ██║       ██║██╔══██╗██║     \r\n") \
	(std::string(HOST) + " 372 " + NICKNAME + " :██║        ██║ ████╗ ██║██║  ██║╚██████╗\r\n") \
	(std::string(HOST) + " 372 " + NICKNAME + " :╚═╝        ╚═╝  ╚══╝ ╚═╝╚═╝  ╚═╝ ╚═════╝\r\n")
#define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) \
	(std::string(HOST) + " 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
#define RPL_ISUPPORT(client, tokens) \
	(std::string(HOST) + " 005 " + client + " " + tokens + " :are supported by this server\r\n")


// Error messages
#define RPL_INVITING(nickname, target) \
	(std::string(HOST) + " 341 " + nickname + " " + target + " :Inviting user to channel\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) \
	(":" + getHostname() + " 403 " + client.getNick() + " " + channel + " :No such channel\r\n")
#define ERR_UNKNOWNCOMMAND(client, command) \
	(std::string(HOST) + " 421 " + client + " " + command + " :Unknown command\r\n")
#define ERR_NOTONCHANNEL(client, channel) \
	(std::string(HOST) + " 442 " + client + " " + channel + " :You're not on that channel\r\n")

/* :localhost 443 lu pastilhex #ivo :is already on channel\r\n */
#define ERR_USERONCHANNEL(client, nick, channel) \
	(std::string(HOST) + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n")

#define ERR_NEEDMOREPARAMS(client, command) \
	(std::string(HOST) + " 461 " + client.getNick() + " " + command + " :Not enough parameters.\r\n")

// JOIN
#define RPL_JOIN(client, channelName) \
	(":" + client.getNick() + " JOIN " + channelName + "\r\n")
#define ERR_INVITEONLYCHAN(client, channel) \
	(":" + getHostname() + " 473 " + client.getNick() + " " + channelName + " :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(client, channel) \
	("474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(client, channel) \
	("475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

// KICK
#define ERR_USERNOTINCHANNEL(client, nickname, channel) \
	(" 441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")
#define RPL_KICK(client, channel, kicked, reason) \
	(":" + client.getNick() + "!" + client.getUsername() + "@" + getHostname() + " KICK " + channel + " " + kicked + " " + reason + "\r\n")

// KILL
#define ERR_NOPRIVILEGES(client) \
	(std::string(HOST) + " 481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
#define RPL_KILL(user_id, killed, comment) \
	(user_id + " KILL " + killed + " " + comment + "\r\n")

// MODE
#define RPL_UMODEIS(client, mode) \
	(std::string(HOST) + " 221 " + client + " " + mode + "\r\n")
#define MODE_USERMSG(client, mode) \
	(":" + client + " MODE " + client + " :" + mode + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) \
	(std::string(HOST) + " 501 " + client + " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(client) \
	(std::string(HOST) + " 502 " + client + " :Cant change mode for other users\r\n")
#define MODE_CHANNELMSG(channel, mode) \
	(std::string(HOST) + "MODE #" + channel + " " + mode + "\r\n")
#define MODE_CHANNELMSGWITHPARAM(channel, mode, param) \
	(std::string(HOST) + "MODE #" + channel + " " + mode + " " + param + "\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode) \
	(std::string(HOST) + " 324 " + client + " #" + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) \
	(std::string(HOST) + " 324 " + client + " #" + channel + " " + mode + " " + password + "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) \
	(std::string(HOST) + " 404 " + client + " #" + channel + " :Cannot send to channel\r\n")
#define ERR_CHANNELISFULL(client, channel) \
	(std::string(HOST) + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) \
	(std::string(HOST) + " 482 " + client.getNick() + " " + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(client, channel, mode, password) \
	(std::string(HOST) + " 696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
#define RPL_ADDVOICE(nickname, username, channel, mode, param) \
	(":" + nickname + "!" + username + "@localhost MODE #" + channel + " " + mode + " " + param + "\r\n")

// MOTD
#define ERR_NOSUCHSERVER(client, servername) \
	(std::string(HOST) + " 402 " + client + " " + servername + " :No such server\r\n")
#define ERR_NOMOTD(client) \
	(std::string(HOST) + " 422 " + client + " :MOTD File is missing\r\n")
#define RPL_MOTDSTART(client, servername) \
	(std::string(HOST) + " 375 " + client + " :- " + servername + " Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) \
	(std::string(HOST) + " 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) \
	(std::string(HOST) + " 376 " + client + " :End of /MOTD command.\r\n")

// NAMES
#define RPL_NAMREPLY(client, channel, list_of_nicks) \
	(std::string(HOST) + " 353 " + client.getNick() + " = " + channel + " :" + list_of_nicks + "\r\n")
#define RPL_ENDOFNAMES(client, channel) \
	(std::string(HOST) + " 366 " + client.getNick() + " " + channel + " :End of /NAMES list.\r\n")

//LIST
#define RPL_LIST(client, nbrUserStr, it) \
	(":" + this->getHostname() + " 322 " + client.getNick() + " " + channelName + " " + std::string(nbrUserStr) + " :" + it->second.getTopic() + "\r\n")
#define RPL_LISTEND(client) \
	(":" + this->getHostname() + " 323 " + client.getNick() + " :End of /LIST\r\n")
	
// NICK
#define ERR_NONICKNAMEGIVEN(client) \
	(std::string(HOST) + " 431 " + client + " :There is no nickname.\r\n")
#define ERR_ERRONEUSNICKNAME(client, nickname) \
	(std::string(HOST) + " 432 " + client + " " + nickname + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(hostname, nickname) \
	(":" + hostname + " 433 " + nickname + " :Nickname is already in use.\r\n")
#define RPL_NICK(oclient, uclient, client) \
	(":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

// NOTICE
#define RPL_NOTICE(nick, username, target, message) \
	(":" + nick + "!" + username + "@localhost NOTICE " + target + " " + message + "\r\n")

// OPER
#define ERR_NOOPERHOST(client) \
	(" 491 " + client + " :No O-lines for your host\r\n")
#define RPL_YOUREOPER(client) \
	(" 381 " + client + " :You are now an IRC operator\r\n")

// PART
#define RPL_PART(channel, reason) \
	(USER_ID(NICKNAME, USERNAME) + " PART " + channel + " " + (reason.empty() ? "Whitout any reason" : reason) + "\r\n")

// PASS
#define ERR_PASSWDMISMATCH(client) \
	(std::string(HOST) + " 464 " + client.getNick() + " :Server password incorrect.\r\n")

// PING :pastilhex!ivo@localhost PONG :LAG1713199744574
#define RPL_PONG(nickname, username, token) (USER_ID(nickname, username) + " PONG :" + token + "\r\n")

// QUIT
#define RPL_QUIT(user_id, reason) \
	(user_id + " QUIT :Quit: " + reason + "\r\n")
#define RPL_ERROR(user_id, reason) \
	(user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
#define ERR_NOSUCHNICK(client, target) \
	(":" + getHostname() + " 401 " + client.getNick() + " " + target + " :No such nick/channel\r\n")
#define ERR_NORECIPIENT(client) \
	(" 411 " + client + " :No recipient given PRIVMSG\r\n")
#define ERR_NOTEXTTOSEND(client) \
	(" 412 " + client + " :No text to send\r\n")
#define RPL_PRIVMSG(channel, message) \
	(USER_ID(NICKNAME, USERNAME) + " PRIVMSG " + ((channel == "") ? input[1] : channel) + " :" + message + "\r\n")

// TOPIC
#define RPL_TOPIC(client, channel) \
	(std::string(HOST) + " 332 " + client.getNick() + " " + channel.getName() + " " + channel.getTopic() + "\r\n")
#define RPL_NOTOPIC(client, channel) \
	(std::string(HOST) + " 331 " + client.getNick() + " " + channel.getName() + " :No topic is set\r\n")

// USER
#define ERR_ALREADYREGISTERED(client) \
	(std::string(HOST) + " 462 " + client + " :You may not reregister.\r\n")

#endif