NAME = FT_IRC

SRC = ./srcs/channel.cpp ./srcs/client.cpp ./srcs/server.cpp ./srcs/commands.cpp main.cpp

HEADER = headers/server.hpp headers/channel.hpp headers/client.hpp

OBJ = $(SRC:%.cpp=%.zizi)

RM = rm -rf

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -MD -MP

DEPENDS = $(OBJ:.zizi=.d)

CC = c++

all: $(NAME)

%.zizi: %.cpp $(HEADER) Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf	"\033[1;33m \r\033[2KCreating -c $< -o $\n \033[0m"

$(NAME): $(OBJ) $(HEADER) Makefile
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf	"\033[1;32m \r\033[2K Compiling $(NAME) : DONE \033[0m\n"
-include $(DEPENDS)

clean:
	@$(RM) $(OBJ) $(TEMPLATE_OBJ) $(DEPENDS)
	@printf	"\033[1;31m \r\033[2KDeleting objects : DONE \033[0m\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "\033[1;31m \r\033[2KDeleting executable : DONE \033[0m\n\n"

re: fclean all

.PHONY: all clean fclean re
