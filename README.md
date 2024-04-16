42 Project ft_irc
# Internet Relay Chat

Summary:
This project is about creating your own IRC server.
You will use an actual IRC client to connect to your server and test it.
Internet is ruled by solid standards protocols that allow connected computers to interact
with each other.
It’s always a good thing to know.


Functions Used: 
- [x] socket
- [x] close
- [ ] setsockopt
- [ ] getsockname
- [ ] getprotobyname
- [x] gethostbyname
- [ ] getaddrinfo
- [ ] freeaddrinfo
- [x] bind
- [ ] connect
- [x] listen
- [x] accept
- [x] htons, htonl
- [ ] ntohs, ntohl
- [ ] inet_addr
- [x] inet_ntoa
- [x] send
- [x] recv
- [x] signal, sigaction
- [ ] lseek
- [ ] fstat
- [ ] fcntl
- [x] poll

## Mandatory
- [x] You must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages using your reference client.
- [x] All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
- [x] You must have operators and regular users.
**Then, you have to implement the commands that are specific to channel operators:**
	- [x] KICK - Eject a client from the channel
	- [x] INVITE - Invite a client to a channel
	- [x] TOPIC - Change or view the channel topic
	- [x] MODE - Change the channel’s mode:
		- [x] i: Set/remove Invite-only channel
		- [x] t: Set/remove the restrictions of the TOPIC command to channel operators
		- [ ] k: Set/remove the channel key (password)
		- [x] o: Give/take channel operator privilege
		- [ ] l: Set/remove the user limit to channel

## Bonus - Internet Relay Chat
- [ ] Handle file transfer
- [ ] A bot
