NAME= 21sh
LIBFT_PATH= ./libft/
INCLUDE= -I ./includes/  -I $(LIBFT_PATH)
INC = ./includes/21sh.h ./includes/21sh_r.h
SRC_DIR_PARSE= ./SRC_HEAD/src/
SRC_DIR_READLINE= ./SRC_HEAD/readline/
OBJ_DIR_READLINE= ./obj_readline/
OBJ_DIR_HEAD= ./obj_HEAD/

FLAGS= -g -Wall -Wextra -Werror

SRC_NAME= main.c \
			envirement.c \
			error_handler.c \
			node.c \
			redirections.c \
			utils.c \
			utils2.c \
			parse_pipe.c \
			printing.c \
			expansion.c \
			split.c \
			expand.c \
			free1.c \
			quote_completion.c \
			here_doc.c \
			ft_free.c \
			ast.c \
			check_grammar.c \

SRC_NAME2= ft_readline.c \
			history.c \
			keys_alt.c \
			move_cursor.c \
			selection.c \

SRCS = $(addprefix $(SRC_DIR_PARSE), $(SRC_NAME)) 
OBJS = $(addprefix $(OBJ_DIR_HEAD), $(SRC_NAME:.c=.o))
SRCS2 = $(addprefix $(SRC_DIR_READLINE), $(SRC_NAME2))
OBJS2 = $(addprefix $(OBJ_DIR_READLINE), $(SRC_NAME2:.c=.o))

#VAR = $(SRCS) $(SRCS2)
OBJECT = $(OBJS) $(OBJS2)
all: $(NAME)

$(NAME): $(OBJECT) $(INC)
	@make -sC $(LIBFT_PATH)
	@gcc $(FLAGS) $(OBJECT) $(INCLUDE) -L $(LIBFT_PATH) -lft -ltermcap -o $(NAME)
	@echo "\033[1m\033[35m|⩺▾ 21sh Successfully Created ☯ ⩹|\033[0m"
$(OBJ_DIR_HEAD)%.o: $(SRC_DIR_PARSE)%.c
	@mkdir -p obj_HEAD
	@gcc -c $^ $(INCLUDE) -o $@
	@echo "\033[92m|⩺  Object file Created ⩹|"
$(OBJ_DIR_READLINE)%.o: $(SRC_DIR_READLINE)%.c
	@mkdir -p obj_readline
	@gcc -c $^ $(INCLUDE) -o $@
	@echo "\033[92m|⩺  Object file Created ⩹|"
clean:
	@echo "\033[1m\033[93m|⩺  cleaning...\033[0m"
	@make clean -sC  $(LIBFT_PATH)
	@rm -rf $(OBJ_DIR_HEAD) $(OBJ_DIR_READLINE)
fclean: clean
	@make fclean -sC $(LIBFT_PATH)
	@rm -f $(NAME)
	@echo "\033[92m|⩺  21sh executable & object files REMOVED!\033[0m"
re: fclean all
