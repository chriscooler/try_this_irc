# Colors
GREY = \x1b[30m
RED = \x1b[31m
GREEN = \x1b[32m
YELLOW = \x1b[33m
BLUE = \x1b[34m
PURPLE = \x1b[35m
CYAN = \x1b[36m

NAME			= ircserv

UNAME			:= $(shell uname)

ifeq ($(UNAME),Darwin)
	OS = -D MACOS
endif

ifeq ($(UNAME),Linux)
	OS = -D LINUX
endif

CC				= clang++

CFLAGS  		= -I ./headers -Wall -Wextra -Werror -fsanitize=address -g3

CPPFLAGS		= -std=c++98

PATH_HEADER		= headers/

PATH_SRCS		= srcs/

PATH_OBJS		= objs/

HEADER_LIST		= 

SRC_LIST		= main.cpp \
			      Stock.cpp \
				  error.cpp \
				  server.cpp \
				  parse_message.cpp \
				  passCmd.cpp \
				  nickCmd.cpp \
				  userCmd.cpp \
				  error.cpp \
				  Join.cpp \
				  Privmsg.cpp

OBJ			= $(addprefix $(PATH_OBJS), $(SRC_LIST:.cpp=.o))

HEADER			= $(addprefix $(PATH_HEADER), $(HEADER_LIST))

RM				= rm -rf

all: ${NAME}

$(PATH_OBJS)%.o : $(PATH_SRCS)%.cpp $(HEADER)
	
	@mkdir -p $(PATH_OBJS) 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ 

$(NAME): $(OBJ) $(HEADER)

	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) -o $(NAME)

re:	fclean all

clean:
	@${RM} ${PATH_OBJS}
	@printf "$(YELLOW) *.o $(RED)delete.\n"

fclean:	clean
	@${RM} $(NAME)
	@printf "$(YELLOW) *.o, $(GREEN)${NAME} $(RED)delete.\n"

.PHONY: all clean fclean re
