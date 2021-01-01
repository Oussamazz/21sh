/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 23:01:13 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 14:18:15 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H

# include "libft.h"
# include "readline.h"
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
# define METACHARACTER " \n\t;"
# define PIPE "|"
# define AGG_REDI "<>&"
# define DEFAULT_FD_OUT "1"
# define DEFAULT_FD_IN "0"
# define ERR_BAD_FD "21sh: bad file descriptor.\n"
# define MAX_INDEX 4096
# define MIN_INDEX 1024
# define WELCOME_MSG0 "\t\033[1;32m⬇⬇  w3lc0m3 70 7h3_qu173r "
# define WELCOME_MSG1 "& ph30n1x 5h3ll ⚙️ ⬇⬇\033[0m\t\t"
# define ERROR2 "21sh: setenv: Variable name must begin with a letter "
# define ERROR22 "and contains only alpha-numeric characters."
# define ERROR4 "21sh: Error: Left redirection must "
# define ERROR44 "only contains digits characters."

# define ERROR1 "21sh: Error: [setenv [var_name] [var_value]]."
# define ERROR3 "Envirenment variables not set."
# define ERROR5 "21sh: cd: permission denied: "
# define ERROR6 "21sh: cd: no such file or directory: "

enum {
	F_TOKENZ,
	F_TMP,
	F_AST,
	F_STR,
	F_G_HIS
};

typedef enum	e_split{
	Normal,
	Continue,
	Break,
	ReturnNull,
	Returnagg
}				t_split;

size_t			g_agg_len;
char			*g_tty_name;
int				g_ex_flag;

typedef struct			s_mypipe
{
	int					pipe[2];
	pid_t				pid;
	int					temp;
	int					cmd_no;
}						t_mypipe;

typedef struct			s_pointt
{
	int					node_index;
	void				*node_addr;
	int					aggr_index;
	int					pipe_index;
	int					no_space;
}						t_pointt;

typedef struct			s_env
{
	char				*env_var_name;
	char				*env_var_value;
	struct s_env		*next;
}						t_env;

typedef enum			e_type
{
	WORD = 1,
	METACHAR,
	L_REDIR,
	AGGR_SYM,
	R_REDIR,
	PIPE_SYM,
	DQUOT,
	SQUOT,
	EXPANSION,
	SEP
}						t_type;

typedef struct			s_quote
{
	char				*string;
	size_t				size;
}						t_quote;

typedef struct			s_lexer
{
	t_pointt			coor;
	char				*data;
	t_type				type;
	struct s_lexer		*next;
}						t_lexer;

typedef struct			s_his
{
	char				*data;
	struct s_his		*next;
}						t_his;

t_his			*g_his;
int				g_alltokenzsize;

typedef struct			s_redir
{
	char				*lfd;
	char				*rfd;
	char				*sym;
	struct s_redir		*next;
}						t_redir;

typedef struct			s_miniast
{
	char				**cmd;
	int					no_space;
	t_redir				*redirection;
	struct s_miniast	*pipe;
	struct s_miniast	*sep;
}						t_miniast;

typedef struct			s_mystruct
{
	int					i;
	int					j;
	int					flag;
	int					position;
	size_t				size;
	char				c;
	char				**arr;
	char				*str;
	char				*tmp;
	int					status[2];
	t_miniast			*ast;
	t_lexer				*tokenz;
	t_env				**env_list;
	t_pointt			coord;
}						t_mystruct;

typedef struct			s_split_redir
{
	size_t				i;
	size_t				j;
	char				**agg;
	char				*tmp;
	int					active_word;
	size_t				delim_len;
	size_t				len;
	size_t				agg_len;
	size_t				agg_len_str;
	int					status;
}						t_split_redir;

typedef	struct			s_getfullcmd
{
	char				*cmd;
	char				*tmp;
	char				*to_free;
	int					quote_opened;
	int					i;
	char				c;
	int					flag;
}						t_getfullcmd;

typedef struct			s_here_doc
{
	int					flag;
	char				*text;
	char				*tmp;
	char				*buffer;
}						t_here_doc;

typedef struct			s_expand
{
	char				**arr;
	char				**value;
	char				*string;
	char				*tmp;
	int					i;
	int					j;
	int					val_size;
}						t_expand;

typedef struct			s_expansion
{
	int					i;
	int					j;
	int					data_size;
	char				*data;
	char				*env_value;
	char				*buf;
}						t_expansion;

typedef struct			s_tilde_exp
{
	int					i;
	char				*home_value;
	char				*user;
	char				*user_name;
}						t_tilde_exp;

typedef struct			s_get_bin
{
	int					i;
	char				*bin_file;
	char				*env_path_value;
	char				**dirs;
	char				*tmp;
	char				*tmp2;
}						t_get_bin;

void					source_sh(t_env **head);
char					*here_doc(char *delim);
void					append_list(t_lexer **root, char *data, t_type type,
	t_pointt *cor);
void					append_list_redi(t_lexer **root, char *data,
	t_type type, t_pointt *cor);
void					append_list_pipe(t_lexer **root, char *data,
	t_type type, t_pointt *cor);
t_lexer					*lexer(char *buf, t_env **head, t_pointt *coord);
t_quote					*quote_handling(char *s, char quote, int start);
int						parse_pipe(t_lexer **token_node, char *str,
	t_pointt *coor);
int						check_tokenz_grammar(t_lexer *tokenz);
int						expansion_function(char *buf, t_lexer **tokenz,
	t_pointt *coord, t_env **env_list);
int						sep_function(t_lexer **tokenz, t_pointt *cor);
int						aggr_function(char *str, t_pointt *cor,
	t_lexer **lexer);
int						quote_function(char *buf, t_lexer **tokenz,
	t_pointt *cord, char *quote);
int						quote_handling_function(t_lexer **token_node,
	t_quote *quot, char quote, t_pointt *coord);
int						simple_word_function(char *buf,
	t_lexer **token_node, t_pointt *coord);
int						execute(t_miniast *tree, t_env **env_list);
int						execute_pipes(t_miniast *tree,
	char **tabs, t_env **env_list);
void					execute_direct(char **cmd, char **tabs);
void					execute_undirect(char **cmd, char **tabs, t_env **env);
void					execute_blt_without_fork(t_miniast *tree,
	char **cmd, char **tabs, t_env **env_list);
void					execute_blt_with_fork(char **cmd, char **tabs,
	t_env **env_list);
int						execute_redirection(t_redir *redirections,
	char *tty_name);
int						ft_redirect_in_out(t_redir *redirections,
	t_redir *prev, int fd);
int						append_redir(t_redir *redirection, t_redir *prev);
int						here_document(t_redir *redirection, char *tty_name);
int						agg_redirection(t_redir *redirections,
	t_redir *prev, int fd);
bool					is_quote(int c);
size_t					ft_strlen_char(char *s, char c);
size_t					ft_strlen_char2(char *s, char c, char c2);
size_t					ft_strlen_char_2(char *s, char c, char c2);
char					*ft_strchr_blank(const char *str, int c);
int						check_command_redir(t_lexer **head, char *buf,
	t_pointt *cor);
t_type					last_node_type(t_lexer *tokenz);
char					**strsplit(char const *s);
int						ft_is_aggr(char c);
t_quote					*quote_completion(t_quote **data, char quote);
size_t					get_list_size(t_lexer *tokenz);
char					*get_left_fd_(char *buf);
char					**list_to_tabs(t_env **env_list);
char					*get_bin_file(char **cmd, t_env **env);
int						env_exist(t_env **env_list, char *var_name);
void					modify_env(t_env **env_list, char *var_name,
	char *var_value);
char					*get_cwd(void);
void					gen_oldpwd(char *cwd, t_env **env_list);
void					gen_pwd(char *new_path, t_env **env_list);
int						ft_str_is_digit(char *lfd);
int						check_builtins(char *cmd_name);
int						check_args_no(char **cmd);
void					type_builtin(char **cmd, t_env **env_list);
char					*join_all_bufs(t_his *his);
int						str_is_blank(char *buffer);
char					*get_content_quote(char *buffer,
	char c, t_pointt *coord, int flag_c);
int						check_builtins_nfrk(char *cmd_name);
int						get_the_word(char *buf,
	t_lexer **token_node, t_pointt *coord);
size_t					get_arr_size_tokenz(t_lexer *token);
int						ft_agg_digit(t_redir *redirection, int fd, int lfd);
int						ft_agg_close(t_redir *redirection,
	int fd, int lfd);
int						ft_agg_word(t_redir *redirection,
	t_redir *prev, int fd, int lfd);
char					*get_dollars(char *buf);
char					*get_splitter(char *buf, t_mystruct *v);
int						ft_agg_out(t_redir *redir, t_redir *prev, int fd);
int						ft_agg_in(t_redir *redir, t_redir *prev, int fd);
size_t					countall_arr(char **agg, size_t delime_len);
char					*redirection_varname(char ***arr, char *str, size_t *i);
int						split_varname(t_split_redir *v, char *str);
int						split_herdoc(t_split_redir *v, char *str);
void					ft_ctrlc(int sig_no);
char					*handel_signal(t_getfullcmd *v);
void					flag_g(char **av, t_env **env_list, time_t *now);
int						ft_str_is_digit(char *lfd);
void					ft_reset_fd(char *tty_name, int file_d);
int						parse_commands(t_miniast **head,
	t_lexer *tokenz, t_env **env);
char					**fill_node(t_lexer *token,
	t_redir **redirections, t_env **env, int alltoken_size);
int						check_grammar_tokenz(t_lexer *tokenz);
int						fill_cmd_redir(t_lexer *token,
	int *i, t_redir **redirections);
void					fill_cmd(char **ret,
	t_lexer *token, int *i, t_env **env);
void					fill_redirections(t_redir **node, t_lexer *token);
size_t					calc_arr_size(t_lexer *token, int *next_type);
void					stock_env(char **env, t_env **head);
int						addtolist(t_env **head,
	char *var_name, char *var_value);
void					deletenode(t_env **head_ref, char *env_name);
char					**split_redir(char *str);
size_t					redirerction_parse(t_lexer **token_node,
	char **agg, t_pointt *cor);
void					ft_reset_fd(char *tty_name, int file_d);
void					blt_echo(char **cmd);
void					blt_setenv(char **cmd, t_env **env_list);
void					blt_unsetenv(char **cmd, t_env **env_list);
void					blt_cd(char **cmd, t_env **env_list);
char					*get_history(t_lexer *tokenz);
int						expansion_parse(t_lexer **token_node,
	char *buf, t_env **env_list, t_pointt *cor);
char					*get_value_expansion(char *env_name, t_env **head);
int						ft_is_expansion(char *str);
int						ft_is_tilde(char *str);
char					*expanded(t_env **head, char *str);
void					tilde_exp(char *exp, char **data, t_env **env_list);
void					print_env_list(t_env **head);
void					starting_message(char **argv, char **user, time_t *now);
void					print_arr(char **arr);
void					print_btree(t_miniast *ast);
void					ft_putendl_fd_error(char *s1,
	char *s2, char *s3, char *s4);
void					prompt_completion(char quot);
void					ft_put_multistring2(char **strings);
void					ft_free_arr(char **arr);
void					ft_free_tokenz(t_lexer **head);
void					ft_free_tree(t_miniast **tree);
void					ft_free_his(t_his **g_his);
char					*err_ret(char *s, char *addrr);
void					error_message(char *err_message, int flag);
int						print_error_sym(t_type type);
void					exit_blt(t_miniast **root, t_lexer **lex,
	t_env **env_list, char **buff);
void					free_env_list(t_env **head);
void					free_vars(t_mystruct *v, int *to_free, int size);

#endif
