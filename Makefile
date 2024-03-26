NAME = ircserv
SRC = main.cpp srcs/ft_messages.cpp srcs/Server.cpp

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