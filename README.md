🚀 **Welcome to 42 IRC Server - A Minimalist IRC Server Implementation** 🚀

## About

The 42 IRC Server project is a minimalist IRC server implementation developed as part of the [42 school](https://www.42porto.com/en/) curriculum. 

The aim is to provide a basic server infrastructure for hosting IRC chat rooms, managing user connections, and facilitating communication between clients. The project is implemented in `C++` and follows the standards and specifications of the [IRC protocol](https://www.ietf.org/rfc/rfc1459.txt).

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

| Command  | Description                                                |
|----------|------------------------------------------------------------|
| /JOIN    | Allows a user to join a specific channel.                  |
| /MODE    | Shows, defines or modifies channel or user modes.          |
| /INVITE  | Invites a user to join a specific channel.                 |
| /PART    | Makes the user leave a specific channel.                   |
| /KICK    | Ejects a client from the channel.                          |
| /TOPIC   | Changes or views the channel topic.                        |
| /PRIVMSG | Sends a private message to a user or a channel.            |
| /QUIT    | Disconnects the user from the server.                      |
| /WHO     | Provides information about a user, a channel, or the server.|
| /LIST    | Lists channels, number of users and their topics.                           |

### Command Details:

### ``JOIN``
  - **Description:** This command is used to join a specific channel on the IRC server.
  - **Example:** `/join #chat-room`

### ``MODE``
  - **Description:** Allows defining or modifying channel or user modes.
  - **Note:** Operators' privilege might be required for some arguments.


    | Argument | Description                          | Example                      |
    |----------|--------------------------------------|------------------------------|
    | no argument       | Shows the channel's modes | `/mode #channel` |
    | i        | Invite Only - Only invited users can join the channel. | `/mode #channel +i` |
    | k        | Password - Sets a password for the channel. | `/mode #channel +k password` |
    | l        | Limit - Limits the number of users that can join the channel. | `/mode #channel +l 10` |

In our system, we use a simple way to enable and disable functionalities. 
Here's a visual representation:

| Symbol | Action | Description |
|--------|--------|-------------|
| `+`    | Set    | Activates a funtionality. This means that whatever functionality is tied to the command would be enabled and ready for use. |
| `-`    | Unset  | Deactivates a functionality. This means that the functionality tied to the command would be turned off and will not respond to triggers. |


For instance, in the case of the `mode` command:

- `+k` sets a password (channel key), while `-k` unsets the password.
- `+l` sets a limit, while `-l` unsets the limit.
- `+i` sets the channel to invite-only, while `-i` removes the invite-only restriction.
- `+t` sets the restriction of the TOPIC command to channel operators, while `-t` removes this restriction.
- `+o` gives channel operator privilege, while `-o` takes the privilege away.

### ``INVITE``
  - **Description:** Invites a specific user to join a channel. 
  - **Note:** Operators' privilege is required for this command.
  - **Example:** `/invite user_nick #secret-channel`

### ``PART``
  - **Description:** Makes the user leave the specified channel.
  - **Example:** `/part #channel` or just `/part` (if you're inside the channel)

### ``PRIVMSG``
  - **Description:** Sends a message to a specific user or all users in a channel. 
  - **Note:** If a channel is specified, the message will be broadcast to all users in that channel.
  - **Example:** `/privmsg user_nick Hello there!` or `/privmsg #channel Hello everyone!`
  
### ``QUIT``
  - **Description:** Disconnects the user from the IRC server.
  - **Note:** An optional quit message can be provided, which will be displayed to other users.
  - **Example:** `/quit`

### ``WHO``
  - **Description:** Retrieves, channel, or server information.
  - **Note:** The output depends on the parameters provided. It can list users in a specific channel, show details about a specific user, or list all visible users.
  - **Example:** `/who #channel` or `/who user_nick`

### ``KICK``
  - **Description:** Removes a user from a channel.
  - **Note:** This command requires operator privileges. An optional reason can be provided, which will be displayed to the user.
  - **Example:** `/kick user_nick #channel`
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


## About the Macros File

The [Macros.hpp](includes/Macros.hpp) file is key in our system. It contains macros for messages we send to the client, adhering to the IRC protocol's specific syntax. 

This protocol, like a common language, ensures the server and client understand each other. By using this protocol they can understand what actions to perform based on the messages they receive.

Macros abstract away message formatting details, making our code cleaner and easier to maintain. Any changes to the message format only need to be made in one place, improving maintainability.


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
