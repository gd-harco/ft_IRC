# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gd-harco <gd-harco@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/07 20:23:57 by gd-harco          #+#    #+#              #
#    Updated: 2023/12/01 17:42:27 by gd-harco         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME =	ircserv

DEBUG_NAME = ${NAME}_DEBUG

SRC_PATH =	srcs/

SRC =	main.cpp channel.cpp client.cpp server.cpp commands/CommandsExceptions.cpp \
		commands/User.cpp commands/Pass.cpp commands/Nick.cpp commands/Join.cpp \
		commands/Privmsg.cpp commands/Part.cpp utility.cpp NumericReplies.cpp \
		parsing.cpp commands/Mode.cpp commands/Kick.cpp commands/Invite.cpp

DIR_INCS =	incs/

INCLUDES =\
	$(addprefix -I , $(DIR_INCS))

DIR_BUILD	=	.build/
OBJS		=	$(patsubst %.cpp, $(DIR_BUILD)%.zizi, $(SRC))
DEPS		=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(SRC))
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3
DEBUGFLAGS	=	-fsanitize=address -g3

DEPSFLAGS	=	-MMD -MP
RM			=	rm -rf
AR			=	ar rcs
CXX			=	c++

.PHONY:		all
all:
			$(MAKE) $(NAME)


$(NAME):	$(OBJS)
			@$(CXX) $(CPPFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
			@printf	"\033[1;32m \r\033[2K Compiling $(NAME) : DONE \033[0m\n"

.PHONY:	bonus
bonus:	all

.PHONY:	clean
clean:
		@$(RM) $(DIR_BUILD)
		@printf	"\033[1;31m \r\033[2KDeleting objects : DONE \033[0m\n"

.PHONY:	fclean
fclean:	clean
		@$(RM) $(NAME)
		@printf "\033[1;31m \r\033[2KDeleting executable : DONE \033[0m\n"

.PHONY:	re
re:		fclean
		$(MAKE) all

-include $(DEPS)

$(DIR_BUILD)%.zizi : $(SRC_PATH)%.cpp
		@mkdir -p $(shell dirname $@)
		@$(CXX) $(CPPFLAGS) $(DEPSFLAGS) $(INCLUDES) -c $< -o $@
		@printf	"\033[1;33m \r\033[2KCreating -c $< -o $\n \033[0m"
