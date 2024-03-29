NAME = ircserv
SRC = main.cpp srcs/ft_messages.cpp srcs/Server.cpp srcs/Client.cpp
OBJDIR = objs
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
CXX = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(NAME)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(OBJ)

re: fclean all

.PHONY: all re fclean clean