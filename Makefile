.SILENT:

NAME = ircserv

# Compilador
CXX = c++ -g
CXXFLAGS = -std=c++98
WWWFLAGS = -Wall -Wextra -Werror
SANITIZE_FLAGS = -pedantic-errors -fsanitize=address

# Diretórios
SRCDIR = srcs
OBJDIR = objs

# Arquivos fonte
SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/cmds/*.cpp) $(wildcard $(SRCDIR)/bot/*.cpp) main.cpp
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Contagem de compilação
COMPILE_COUNT = 0
NUM_SRCS = $(words $(SRCS))

all: $(NAME)

# Compilação
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(WWWFLAGS) $(OBJS) -o $(NAME)
	@printf "\r[\033[32m100%%\033[0m] \001\033[1;97m\002- $(NAME) compiled successfully! \n \001\033[0m\002"
	@printf "\nUsage \001\033[1;97m\002./$(NAME) <port> <password>\001\033[0m\002\n"

# Regras de compilação
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)/cmds
	@mkdir -p $(OBJDIR)/bot
	@$(CXX) $(CXXFLAGS) $(WWWFLAGS) -c $< -o $@
	@$(eval COMPILE_COUNT=$(shell echo $$(($(COMPILE_COUNT)+1))))
	@printf "\r[\033[32m%3d%%\033[0m] Compiling: $<    " $$(($(COMPILE_COUNT) * 100 / $(NUM_SRCS)))

# Regra especial para compilar main.cpp
$(OBJDIR)/main.o: main.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(WWWFLAGS) -c $< -o $@
	@$(eval COMPILE_COUNT=$(shell echo $$(($(COMPILE_COUNT)+1))))
	@printf "\r[\033[32m%3d%%\033[0m] Compiling: $<    " $$(($(COMPILE_COUNT) * 100 / $(NUM_SRCS)))

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean
