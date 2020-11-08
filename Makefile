NAME= 21sh
LIBFT_PATH= ./libft/
INCLUDE= -I ./includes/  -I $(LIBFT_PATH)
SRC_DIR_PARSE= ./src/
SRC_DIR_READLINE= ./readline/
SRC_HEAD_DIR = ./SRC_HEAD/readline ./SRC_HEAD_DIR/src
OBJ_DIR= ./obj/
FLAGS=  -Wall -Wextra -Werror

SRC_NAME= main.c \
			envirement.c \
			error_handler.c \
			node.c \
			redirections.c \
			utils.c \

SRC_NAME2= ft_readline.c \
			history.c \
			keys_alt.c \
			move_cursor.c \
			selection.c \

SRCS = $(addprefix $(SRC_DIR_PARSE), $(SRC_NAME)) 
OBJS = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))
SRCS2 = $(addprefix $(SRC_DIR_READLINE), $(SRC_NAME2))
OBJS2 = $(addprefix $(OBJ_DIR), $(SRC_NAME2:.c=.o))

VAR = $(SRCS) $(SRCS2)
OBJECT = $(OBJS) $(OBJS2)
all: $(NAME)

$(NAME): $(OBJECT)
	@make -sC $(LIBFT_PATH)
	@gcc $(FLAGS) $(OBJECT) $(INCLUDE) -L $(LIBFT_PATH) -lft -o $(NAME)
	@echo "\033[1m\033[35m|⩺▾ 21sh Successfully Created ☯ ⩹|\033[0m"
$(OBJ_DIR)%.o: $(SRC_HEAD_DIR)%.c
	@mkdir -p obj
	@gcc -c $^ $(INCLUDE) -o $@
	@echo "\033[92m|⩺  Object file Created ⩹|"
clean:
	@echo "\033[1m\033[93m|⩺  cleaning...\033[0m"
	@make clean -sC  $(LIBFT_PATH)
	@rm -rf $(OBJ_DIR)
fclean: clean
	$(VAR)
	@make fclean -sC $(LIBFT_PATH)
	@rm -f $(NAME)
	@echo "\033[92m|⩺  21sh executable & object files REMOVED!\033[0m"
re: fclean all
