🚀 **Welcome to 42 IRC Server - A Minimalist IRC Server Implementation** 🚀

## About

The 42 IRC Server project is a minimalist IRC server implementation developed as part of the 42 school curriculum. The aim is to provide a basic server infrastructure for hosting IRC chat rooms, managing user connections, and facilitating communication between clients. The project is implemented in [programming language] and follows the standards and specifications of the IRC protocol.

To access the complete project description, click [here](https://github.com/Pastilhex/ft_irc/blob/main/en.subject.pdf).

### Contributors - Collaborative Work 🤝 
> We worked together to develop and enhance this project, sharing knowledge and experiences throughout the process :)

- [Ivo Marques](https://github.com/Pastilhex) - **ialves-m**
- [Luiza Zilio](https://github.com/ziliolu) - **lpicoli-**
- [Jhonata Gonçalves](https://github.com/SopadeGalinha) - **jhogonca**

## Features

- **Multi-User Chat:** Host multiple chat rooms simultaneously, allowing users to join and participate in discussions.
- **User Authentication:** Implement user authentication mechanisms to ensure secure access to chat rooms.
- **Message Broadcasting:** Broadcast messages from users to all participants in a chat room.
- **Command Handling:** Support basic IRC commands such as JOIN, PART, PRIVMSG, and more.
- **Bot Support:** Implement a bot that can be used to automate tasks such as welcoming new users or sending messages to users in specific channels.

## Available Commands

Here are the available commands in the IRC protocol supported by the 42_irc server, along with details and usage examples:

| Command  | Description                                                | Example                      |
|----------|------------------------------------------------------------|------------------------------|
| /JOIN    | Allows a user to join a specific channel.                  | /join #channel               |
| /MODE    | Shows, defines or modifies channel or user modes.                 | /mode #channel +i            |
| /INVITE  | Invites a user to join a specific channel.                 | /invite user #channel        |
| /PART    | Makes the user leave a specific channel.                   | /part #channel               |

### Command Details:

- **/ JOIN**
  - **Description:** This command is used to join a specific channel on the IRC server.
  - **Example:** `/join #chat-room`

- **/ MODE**
  - **Description:** Allows defining or modifying channel or user modes.
  - **Note:** Operators' privilege might be required for some arguments.


    | Argument | Description                          | Example                      |
    |----------|--------------------------------------|------------------------------|
    | no argument       | Shows the channel's modes | `/mode #channel` |
    | i        | Invite Only - Only invited users can join the channel. | `/mode #channel +i` |
    | k        | Password - Sets a password for the channel. | `/mode #channel +k password` |
    | l        | Limit - Limits the number of users that can join the channel. | `/mode #channel +l 10` |

- **/ INVITE**
  - **Description:** Invites a specific user to join a channel. 
  - **Note:** Operators' privilege is required for this command.
  - **Example:** `/invite user_nick #secret-channel`

- **/ PART**
  - **Description:** Makes the user leave the specified channel.
  - **Example:** `/part #channel` or just `/part` (if you're inside the channel)

## Bot

### Commands 
| Command      | Description                                             | Additional Requirements     |
|--------------|---------------------------------------------------------|-----------------------------|
| ``!create``  | Create a bot in a specific channel.                    | Operator's privilege required |
| ``!delete``  | Delete a bot from a specific channel.                  | Operator's privilege required |
| ``!help``    | Display help information.                               |                             |
| ``!funfact`` | Show a random fun fact.                                        |                             |
| ``!quote``   | Display a random inspiring quote.                                 |                             |

### Welcome and Goodbye Messages

- **Welcome:** Upon joining a channel with the bot, users are greeted.
- **Goodbye:** When users leave the channel, the bot bids them farewell.


## Installation

To install and run the 42 IRC Server, follow these steps:

1. Clone the repository:

   ```bash
   	git clone git@github.com:your-username/42_irc.git
   ```

2. Navigate to the project directory:

   ```bash
   	cd 42_irc
   ```

3. Build and compile the server:

   ```bash
   	make
   ```

4. Start the server:

   ```bash
   	./ircserv <port> <password>
   ```

## Usage

Once the server is running, users can connect to it using an IRC client application like [WeeChat](https://weechat.org/), [HexChat](https://hexchat.github.io/) or [Irssi](https://irssi.org/). They can then join chat rooms, send messages, and interact with other users in real-time.

## Disclaimer

Maybe you could notice a different coding style.
[Click here](https://github.com/MagicHatJo/-42-Norm/blob/master/norme.en.pdf) to read the norm file of 42 school. 