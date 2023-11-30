END		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
CYAN 	= \033[1;36m

RM		= rm -f
NAME	= minishell
INCLUDES = include

DIR_LIBFT = srcs/libft
INCLUDE_LIB = $(DIR_LIBFT)/include
LIB = $(DIR_LIBFT)/libft.a
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES) -I $(INCLUDE_LIB) -g #-fsanitize=address

SRCDIR	= srcs
OBJDIR	= objs

SRC		= minishell_utils.c\
		  minishell.c \
		  parse/lexer.c \
		  parse/parse.c \
		  parse/parse_utils.c \
		  minishell_exec.c \
		  builtin/builtin.c \
		  builtin/builtin2.c \
		  builtin/ep_lst.c \
		  builtin/ep_lst2.c

SRC		:= $(addprefix srcs/,$(SRC))

OBJ		= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

all:	$(NAME)

$(LIB): | $(OBJDIR)
	@make -s -C $(DIR_LIBFT)

$(NAME): $(LIB) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -lreadline
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)Stuff compiled 🛠️\n$(END)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(LIB)
	@mkdir -p $(@D)
	@cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)/*.o
	@make -s -C $(DIR_LIBFT) clean

fclean:	clean
	@$(RM) $(NAME)
	@$(RM)r objs
	@make -s -C $(DIR_LIBFT) fclean
	@echo "\n$(BLUE)$(NAME)$(END) $(GREEN)All stuff removed 🗑️\n$(END)"

re:	fclean all

run: ${NAME}
	@./${NAME}

VALG	= valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes# --trace-children=yes

v:
	@make
	@${VALG} ./${NAME}

val: ${NAME}
	@make
	@output=$$(${VALG} ./${NAME} 2>&1); \
	if echo "$$output" | grep -q 'freed' && echo "$$output" | grep -q '0 errors' ; then\
		echo -n "$(GREEN)"; echo "$$output" | grep -E 'freed|total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	else\
		echo -n "$(RED)"; echo "$$output" | grep -E 'total|ERROR S|file descriptor' | sed 's/^[^ ]* //';\
	fi;

e: fclean

.PHONY:	all clean fclean re run v val e