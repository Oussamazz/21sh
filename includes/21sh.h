/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 23:01:13 by macos             #+#    #+#             */
/*   Updated: 2020/12/26 11:14:13 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
#define SH_H

# include "libft.h"
# include "21sh_r.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <limits.h>
# include <string.h>
# include <signal.h>
// #include <readline/readline.h>
// #include <readline/history.h>
# define METACHARACTER " \n\t;"
# define PIPE "|"
# define AGG_REDI "<>&"
# define DEFAULT_FD_OUT "1"
# define DEFAULT_FD_IN "0"
# define ERR_BAD_FD "21sh: bad file descriptor.\n"
# define MAX_INDEX 4096
# define WELCOME_MSG "\t\033[1;32m⬇⬇  w3lc0m3 70 7h3_qu173r & ph30n1x 5h3ll ⚙️  ⬇⬇\033[0m\t\t"

// free variables enum
enum{
	F_TOKENZ,
	F_TMP,
	F_AST,
	F_STR,
	F_G_HIS
};
// split_redir status enum
enum{
	Normal,
	Continue,
	Break,
	ReturnNull,
	Returnagg
};


size_t	g_agg_len;
char	*g_tty_name;
int		prompt_flag;
int		g_ex_flag;



/*
** nodes index
*/

typedef struct	s_mypipe
{
	int pipe[2];
	pid_t pid;
	int temp;
	int cmd_no;
}				t_mypipe;



typedef struct s_pointt
{
	int node_index;
	void *node_addr;
	int aggr_index;
	int pipe_index;
	int no_space;
}               t_pointt;

/*
** Environment struct
*/

typedef struct s_env
{
	char *env_var_name;
	char *env_var_value;
	struct s_env *next;
}               t_env;


typedef enum e_type
{
	WORD=1,    // 1 - 1
	METACHAR,  // 0
	L_REDIR,   // 1 - 0
	AGGR_SYM, // >  // 1 - 0
	R_REDIR,   // 1 - 0
	PIPE_SYM,  // 0
	DQUOT,     // 1 - 1
	SQUOT,     // 1 - 1
	EXPANSION, // 1 - 1
	SEP        // 0
}           t_type;

/*
** Quote_hanlding
*/

typedef struct s_quote
{
	char	*string;
	size_t	size;
}               t_quote;

/*
** lexer and parser
*/

typedef struct s_lexer // tokenz
{
	t_pointt coor;
	char *data;         //ls -la | sksks | echo ; sososkos
	t_type type;

	struct s_lexer *next;
}               t_lexer;

typedef struct s_his
{
	char *data;
	t_type type;
	struct s_his *next;
}				t_his;
t_his *g_his;
/*
** redirections struct
*/

typedef struct s_redir
{
	char *lfd;
	char *rfd;
	char *sym;
	struct s_redir *next;
}               t_redir;

/*
** Abstract syntax tree (execution)
*/

typedef struct s_miniast 
{
	char **cmd;
	int no_space;
	t_redir *redirection;
	struct s_miniast *pipe;
	struct s_miniast *sep;
}              t_miniast;

typedef struct s_mystruct
{
	int i;
	int j;
	int flag;
	int position;
	size_t size;
	char c;
	char **arr;
	char *str;
	char *tmp;
	int status[2];
	t_miniast *ast;
	t_lexer *tokenz;
    t_quote *quot;
    t_quote *rec_quot;
	t_env 	**env_list;
	t_pointt coord;
}				t_mystruct;

typedef struct	s_split_redir
{
    int i;
    int j;
    char **agg;
    char *tmp;
    int active_word;
    size_t delim_len;
	size_t len;
	size_t agg_len;
    size_t agg_len_str;
	int status;
}				t_split_redir;


/*
** principale functions
*/
void    	source_sh(t_env** head);
char    	*here_doc(char *delim);
void    	append_list(t_lexer **root, char *data, t_type type, t_pointt *cor);
void    	append_list_redi(t_lexer **root, char *data, t_type type, t_pointt *cor);
void    	append_list_pipe(t_lexer **root, char *data, t_type type, t_pointt *cor);
void    	print_list(t_lexer *token_list); // delete this func
void    	print_list2(t_redir  *redirections); // delete this func
t_lexer 	*lexer(char *buf, t_env **head, t_pointt *coord);
t_quote     *quote_handling(char *s, char quote, int start);
int      	parse_pipe(t_lexer **token_node, char *str, t_pointt *coor);
int     	check_tokenz_grammar(t_lexer *tokenz);

// norme
int     expansion_function(char *buf, t_lexer **tokenz, t_pointt *coord, t_env **env_list);
int   	sep_function(char *buf, t_lexer ** , t_pointt *);
int  	aggr_function(char *, t_pointt *, t_lexer **);
int     quote_function(char *buf, t_lexer ** ,t_pointt *);
int     quote_handling_function(t_lexer **token_node, t_quote *quot, char quote, t_pointt *coord);
int		simple_word_function(char *buf, t_lexer **token_node, t_pointt *coord, size_t buf_len);
int     meta_function(char *buf, t_lexer **token_node, t_pointt *coord);
/*
** Execution functions
*/
int				execute(t_miniast *tree, t_env **env_list);
int				execute_pipes(t_miniast *tree, char **tabs, t_env **env_list); // PIPES
void			execute_direct(char **cmd, char **tabs);
void			execute_undirect(char **cmd, char **tabs, t_env **env);
void   			execute_blt_without_fork(t_miniast *tree, char **cmd, char **tabs, t_env **env_list);
void    		execute_blt_with_fork(t_miniast *tree, char **cmd, char **tabs, t_env **env_list);
/*
** Redirection fucntions:
*/
int			execute_redirection(t_redir *redirections, char *tty_name);
int			ft_redirect_in_out(t_redir *redirections, t_redir *prev, int fd);
int			append_redir(t_redir *redirection, t_redir *prev);
int			here_document(t_redir *redirection, char *tty_name);
int			agg_redirection(t_redir *redirections, t_redir *prev, int fd);

/*
** utils
*/
bool		is_quote(int c);
size_t		ft_strlen_char(char *s, char c);
size_t      ft_strlen_char2(char *s, char c, char c2);
size_t      ft_strlen_char_2(char *s, char c, char c2);
size_t 		ft_strlen_blank(char *s);
char		*ft_strchr_blank(const char *str, int c);
size_t 		ft_strlen_exp(char *s);
char    	valid_string_quot(char *str);
//size_t last_node_check(t_lexer **tokenz, t_type type, int node_index);
size_t 		ft_strchr_size(char *s, int c);
int 		check_quoting(t_lexer** head, t_type type, int cur_node_index);
size_t 		check_command_redir_size(char *buf);
int			check_command_redir(t_lexer **head, char *buf, t_pointt *cor);
char 		*sub_aggr_sym(char *str);
size_t 		wordinbuff_size(char *str);
size_t 		wordinstr_size(char *str, size_t count);
t_type 		last_node_type(t_lexer *tokenz);
t_lexer		*get_last_node(t_lexer **head);
char    	*get_right_redir(char *str);
size_t 		calc_size_right_redir(char * str);
char		**strsplit(char const *s);
int 		ft_is_aggr(char c);
int			valide_quote_check(char *str);
t_quote		*quote_completion(t_quote **data, char quote);
int 		is_there_in_env(char *str, t_env **env_list);
size_t 		get_list_size(t_lexer *tokenz);
int			check_if_is_aggr(t_lexer **root);
char    	*get_left_fd_(char *buf);
char		**list_to_tabs(t_env **env_list);
char        *get_bin_file(char **cmd,  t_env **env);
int     	env_exist(t_env **env_list, char *var_name);
void    	modify_env(t_env **env_list, char *var_name, char *var_value);
char		*get_cwd(void);
void		gen_oldpwd(char *cwd, t_env **env_list);
void		gen_pwd(char *new_path, t_env **env_list);
int			ft_str_is_digit(char *lfd);
t_type 		ret_last_node_type(t_lexer **head);
int			check_builtins(char *cmd_name);
int 		check_args_no(char **cmd);
void        type_builtin(char **cmd, t_env **env_list);
void		add_to_his(char *buffer, t_his **g_his, int flag);
char    	*join_all_bufs(t_his *his);
int     	str_is_blank(char *buffer);
/*
** btree Functions _________________________________________________________
*/


int    		parse_commands(t_miniast **head, t_lexer *tokenz, t_env **env);
char    	**fill_node(t_lexer *token, t_redir **redirections, t_env **env, size_t alltoken_size);
int     	check_grammar_tokenz(t_lexer *tokenz);



//t_miniast   *fill_miniast(t_lexer **head, t_miniast *pipe, t_type type);
size_t calc_arr_size(t_lexer *token, int *next_type);

/*
** envirement
*/
void    stock_env(char **env, t_env **head);
int		addtolist(t_env **head, char *var_name, char *var_value);
void    deleteNode(t_env **head_ref, char *env_name);

/*
** Redirections
*/

char        **split_redir(char *str);
size_t     redirerction_parse(t_lexer **token_node, char **agg, t_pointt *cor);
void		ft_reset_fd(char *tty_name, int file_d);

/*
** built-ins
*/


void    blt_echo(char **cmd);
void    blt_setenv(char **cmd, t_env **env_list);
void    blt_unsetenv(char **cmd, t_env **env_list);
void    blt_cd(char **cmd, t_env **env_list);

/*
** history
*/

char        *get_history(t_lexer *tokenz);

/*
** expansions and Environment
*/
int			expansion_parse(t_lexer **token_node, char *buf, t_env **env_list, t_pointt *cor);
char        *get_value_expansion(char *env_name, t_env **head);
int         ft_is_expansion(char *str);
int         ft_is_tilde(char *str);
char        *expanded(t_env **head, char *str);
/*
**  printing
*/

void    print_env_list(t_env **head);
void    starting_message(char **argv, char **user, time_t *now);
void    print_arr(char **arr);

void     print_btree(t_miniast *ast);
void    ft_putendl_fd_error(char *s1, char *s2, char *s3, char *s4);

/*
** free functions
*/
void    ft_free_arr(char **arr);
void    ft_free_tokenz(t_lexer **head);
void    free_quot(t_quote **data);
void    ft_free_tree(t_miniast **tree);
void    ft_free_his(t_his **g_his);

/*
** error handling
*/
char  *err_ret(char *s, char *addrr);
void  error_message(char *err_message, int flag);
int		print_error_sym(t_type type);

/*
** EXIT AND FREE
*/
void    exit_blt(t_miniast **root, t_lexer **lex, t_env **env_list ,char **buff);
void    free_env_list(t_env **head);
void free_vars(t_mystruct *v, int *to_free,int size);

#endif