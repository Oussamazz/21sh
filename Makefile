NAME= 21sh
LIBFT_PATH= ./libft/
INCLUDE= -I ./includes/  -I $(LIBFT_PATH)
INC = ./includes/21sh.h ./includes/21sh_r.h

SRC_DIR_PARSE:= ./SRC_HEAD/
OBJ_DIR_HEAD:= ./objs/

FLAGS= -Wall -Wextra -Werror
FLAGS= -g 

SRC_BUILTINS = blt_echo.o execute_builtins.o exit_blt.o type_blt.o

SRC_EXPANSION = expand.o expansion.o 

SRC_PARSING = ast.o check_grammar.o lexer.o lexerCore.o quote_completion.o split.o syntax_checker.o

SRC_PIPE_REDIRECTION = agg_redir.o execute_pipes.o here_doc.o redirections.o split_redir.o

SRC_MAIN =	envirement.o error_handler.o execute.o execute_cmd.o free1.o\
			ft_free.o his.o main.o node.o printing.o utils.o utils2.o quote_handling.o

SRC_READLINE = ft_readline.o history.o keys_alt.o move_cursor.o selection.o 

OBJ_BUILTINS = $(addprefix objs/builtins/, $(SRC_BUILTINS))
OBJ_EXPANSION = $(addprefix objs/expansion/, $(SRC_EXPANSION))
OBJ_PARSING = $(addprefix objs/parsing/, $(SRC_PARSING))
OBJ_PIPE_REDIRECTION = $(addprefix objs/pipe_redirection/, $(SRC_PIPE_REDIRECTION))
OBJ_MAIN = $(addprefix objs/src/, $(SRC_MAIN))
OBJ_READLINE = $(addprefix objs/readline/, $(SRC_READLINE))

OBJECT = $(OBJ_EXPANSION) $(OBJ_BUILTINS) $(OBJ_PARSING) $(OBJ_PIPE_REDIRECTION) $(OBJ_MAIN) $(OBJ_READLINE)

CURRENT_OBJ_DIR = objs

all: $(NAME)


$(NAME): $(OBJECT) $(INC)
	@make -sC $(LIBFT_PATH)
	@gcc $(FLAGS) $(OBJECT) $(INCLUDE) -L $(LIBFT_PATH) -lft -ltermcap -o $(NAME)
	@echo "\033[1m\033[35m|⩺▾ 21sh Successfully Created ☯ ⩹|\033[0m"


$(OBJ_DIR_HEAD)%.o : $(SRC_DIR_PARSE)%.c
	$(eval CURRENT_OBJ_DIR=$(shell sh -c "echo $^ |  cut -d / -f 2"))
	@mkdir -p $(OBJ_DIR_HEAD)/$(CURRENT_OBJ_DIR)
	@gcc $(FLAGS) -c $^ $(INCLUDE) -o $@
	@echo "\033[92m|⩺  Object file Created ⩹|"

clean:
	@echo "\033[1m\033[93m|⩺  cleaning...\033[0m"
	@make clean -sC  $(LIBFT_PATH)
	@rm -rf $(OBJ_DIR_HEAD)

fclean: clean
	@make fclean -sC $(LIBFT_PATH)
	@rm -f $(NAME)
	@echo "\033[92m|⩺  21sh executable & object files REMOVED!\033[0m"

re: fclean all

