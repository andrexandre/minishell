END		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

RM		= rm -f
NAME	= minishell
INCLUDES = include
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDES) -I $(INCLUDE_LIB) -g

DIR_LIBFT = libft
INCLUDE_LIB = $(DIR_LIBFT)/include
LIB = $(DIR_LIBFT)/libft.a

SRCDIR	= srcs
OBJDIR	= objs

SRC		= minishell_utils.c\
		  minishell.c \
		  lexer/lexer.c \


OBJ		= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

all:	$(NAME)

lib:
	@make -s -C $(DIR_LIBFT)

$(NAME): lib $(OBJ) $(LIB)
	@cc $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)Stuff compiled 🛠️\n$(END)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(LIB)
	@cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)/*.o
	@make -s -C $(DIR_LIBFT)  clean

fclean:	clean
	@$(RM) $(NAME)
	@$(RM)r objs
	@make -s -C $(DIR_LIBFT) fclean
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)All stuff removed 🗑️\n$(END)"

re:	fclean all

run: ${NAME}
	@./${NAME}

TESTF	= | cat -e

v:
	@make && valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./${NAME} ${TESTF}

val: ${NAME}
	@output=$$(make re && valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./${NAME} ${TESTF} 2>&1); \
	if echo "$$output" | grep -q 'freed' && echo "$$output" | grep -q '0 errors' ; then\
		echo -n "$(GREEN)"; echo "$$output" | grep -E 'freed|total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	else\
		echo -n "$(RED)"; echo "$$output" | grep -E 'total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	fi;

e: fclean

.PHONY:	all clean fclean re run v val e