NAME= 21sh
LIBFT_PATH= ./libft/
INCLUDE= -I ./includes/ -I $(LIBFT_PATH)
SRC_DIR= ./src/
OBJ_DIR= ./obj/
FLAGS= -g -Wall -Wextra -Werror
SRC_NAME= main.c node.c utils.c envirement.c
SRCS = $(addprefix $(SRC_DIR), $(SRC_NAME))
OBJS = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@make -sC $(LIBFT_PATH)
	@gcc $(FLAGS) $(OBJS) $(includes) -L $(LIBFT_PATH) -lft -o $(NAME) -lreadline
	@echo "\033[1m\033[35m--> 21sh CREATED! <--\033[0m"
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p obj
	@gcc -c $(INCLUDE) $^ -o $@
	@echo "\033[92m--> Object file Created <--"
clean:
	@echo "\033[1m\033[93m-> cleaning...\033[0m"
	@make clean -sC  $(LIBFT_PATH)
	@rm -rf $(OBJ_DIR)
fclean: clean
	@make fclean -sC $(LIBFT_PATH)
	@rm -f $(NAME)
	@echo "\033[92m-> 21sh executable & object files REMOVED!\033[0m"
re: fclean all