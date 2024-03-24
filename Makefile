NAME = ircserv
SRC = main.cpp \
	srcs/bindSocketAddress.cpp \
	srcs/getServerAddress.cpp \
	srcs/getServerSocket.cpp \
	srcs/listenConnections.cpp \
	srcs/isValidPort.cpp
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