NAME= 21sh
LIBFT_PATH= ./libft/
INCLUDE= -I ./includes/ -I $(LIBFT_PATH)
SRC_DIR_PARSE= ./src/
#SRC_DIR_READLINE= ./readline/
OBJ_DIR= ./obj/
FLAGS=  -Wall -Wextra -Werror
#SRC_NAME2= ft_readline.c / *****READLINE*****
#		history.c /
#		keys_alt.c /
#		move_cursor.c /
#		selection.c /
#mine
SRC_NAME= envirement.c \
			error_handler.c \
			main.c \
			node.c \
			redirections.c \
			utils.c
SRCS = $(addprefix $(SRC_DIR_PARSE), $(SRC_NAME)) 
#SRCS2 = $(addprefix $(SRC_DIR_READLINE), $(SRC_NAME2))
OBJS = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@make -sC $(LIBFT_PATH)
	@gcc $(FLAGS) $(OBJS) $(includes) -L $(LIBFT_PATH) -lft -o $(NAME) -lreadline
	@echo "\033[1m\033[35m|⩺▾ 21sh Successfully Created ☯ ⩹|\033[0m"
$(OBJ_DIR)%.o: $(SRC_DIR_PARSE)%.c
	@mkdir -p obj
	@gcc -c $(INCLUDE) $^ -o $@
	@echo "\033[92m|⩺  Object file Created ⩹|"
clean:
	@echo "\033[1m\033[93m|⩺  cleaning...\033[0m"
	@make clean -sC  $(LIBFT_PATH)
	@rm -rf $(OBJ_DIR)
fclean: clean
	@make fclean -sC $(LIBFT_PATH)
	@rm -f $(NAME)
	@echo "\033[92m|⩺  21sh executable & object files REMOVED!\033[0m"
re: fclean all
