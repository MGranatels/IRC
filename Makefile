NAME		=	ircserv

CXX			= 	c++


SRCS		=	src/main.cpp	\
				src/Sockets.cpp	\
				src/utils.cpp


SRCS_DIR	=	src

OBJS		=	$(subst $(SRCS_DIR), $(OBJS_DIR), $(SRCS:.cpp=.o))

OBJS_DIR	=	obj

INC			=	-I.

CXXFLAGS	=	-std=c++98 -Wall -Wextra -Werror -g -Iincludes/ -Ofast

all:		$(NAME)

$(OBJS_DIR)/%.o :	$(SRCS_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(NAME):	$(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(INC)

clean:
	rm -rf $(OBJS_DIR)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY: all clean fclean