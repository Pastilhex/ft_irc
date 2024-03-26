NAME = ircserv
SRC = main.cpp \
	srcs/ft_bindSocketAddress.cpp \
	srcs/ft_getServerAddress.cpp \
	srcs/ft_getServerSocket.cpp \
	srcs/ft_listenConnections.cpp \
	srcs/ft_isValidPort.cpp \
	srcs/ft_getAddressInfo.cpp \
	srcs/ft_connectClient.cpp \
	srcs/ft_messages.cpp

OBJ = $(SRC:.cpp=.o)
CXX = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(NAME)

fclean: clean
	rm -f $(OBJ)

re: fclean all

.PHONY: all re fclean clean