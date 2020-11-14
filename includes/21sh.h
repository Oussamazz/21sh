/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 23:01:13 by macos             #+#    #+#             */
/*   Updated: 2020/11/14 00:53:08 by macos            ###   ########.fr       */
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
# include <limits.h>
# include <string.h>
# include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
# define METACHARACTER " \n\t;"
# define PIPE "|"
# define AGG_REDI "<>&"
# define DEFAULT_FD_OUT "1"
# define DEFAULT_FD_IN "0"
# define ERR_BAD_FD "21sh: bad file descriptor.\n"
# define MAX_INDEX 4096


/*
** nodes index
*/

typedef struct s_pointt
{
    int node_index;
    void *node_addr;
    int aggr_index;
    int pipe_index;
}               t_pointt;

/*
** Environment 
*/

typedef struct s_env
{
    char *env_var_name;
    char *env_var_value;
    struct s_env *next;
}               t_env;


typedef enum e_type
{
    WORD=1,
    METACHAR,
    L_REDIR,
    AGGR_SYM,
    R_REDIR,
    PIPE_SYM,
    DQUOT,
    SQUOT,
    EXPANSION,
    SEP
}           t_type;

/*
** Quote_hanlding
*/

typedef struct s_quote
{
    char *string;
    size_t size;
}               t_quote;

/*
** lexer and parser
*/

typedef struct s_lexer
{
    t_pointt coor;
    char *data;
    t_type type;

    bool args_status; //
    char **args; //
    struct s_lexer *next;
}               t_lexer;


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
    // t_lexer **tokenz;
    char **cmd;
    t_redir **redirection;
    struct s_miniast *pipe;
    struct s_miniast *sep;
}              t_miniast;



/*
** principale functions
*/
void    source_sh(t_env** head);
void    append_list(t_lexer **root, char *data, t_type type);
void    append_list_redi(t_lexer **root, char *data, t_type type, t_pointt *cor);
void    append_list_pipe(t_lexer **root, char *data, t_type type, t_pointt *cor);
void    print_list(t_lexer *token_list);
t_lexer    *lexer(char *buf, t_env **head);
t_quote *quote_handling(char *s, char quote, int start);
int      parse_pipe(t_lexer **token_node, char *str, t_pointt *coor);

/*
** utils
*/
bool    is_quote(int c);
size_t  ft_strlen_char(char *s, char c);
char	*ft_strchr_blank(const char *str, int c);
char    valid_string_quot(char *str);
//size_t last_node_check(t_lexer **tokenz, t_type type, int node_index);
size_t ft_strchr_size(char *s, int c);
int check_quoting(t_lexer** head, t_type type, int cur_node_index);

size_t check_command_redir_size(char *buf);
int check_command_redir(t_lexer **head, char *buf, t_pointt *cor);
char *sub_aggr_sym(char *str);
size_t wordinbuff_size(char *str);
size_t wordinstr_size(char *str, size_t count);
t_type last_node_type(t_lexer **head);
char    *get_right_redir(char *str);
size_t calc_size_right_redir(char * str);
/*
** envirement
*/
void    stock_env(char **env, t_env **head);


/*
** Redirections
*/

char        **split_redir(char *str, int i);
size_t     redirerction_parse(t_lexer **token_node, char **agg, t_pointt *cor, int *i_p);


/*
** built-ins
*/






/*
** expansions and Environment
*/
int     expansion_parse(t_lexer **token_node, char *buf, t_env **env_list);
char        *get_value_expansion(char *env_name, t_env **head);

/*
**  printing
*/

void    print_env_list(t_env **head);

/*
** error handling
*/
char *error_ret(char *addr, char *err_message);

#endif