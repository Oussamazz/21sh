/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 05:46:44 by macos             #+#    #+#             */
/*   Updated: 2020/11/07 18:57:41 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SH_H
#define SH_H

# include "libft.h"
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
# define METACHARACTER "|;() "
# define AGG_REDI "<>&"
# define DEFAULT_FD_OUT "1"
# define DEFAULT_FD_IN "0"
# define ERR_BAD_FD "21sh: bad file descriptor.\n"
# define MAX_INDEX 4096


/*
** tokenz types
*/

typedef enum e_type
{
    WORD=1,
    METACHAR,
    PATH,
    L_REDIR,
    R_REDIR,
    AGGR_SYM,
    DQUOT,
    SQUOT,
    EXPANSION
}           t_type;

/*
** Environment 
*/

typedef struct s_env
{
    char *env_var_name;
    char *env_var_value;
    struct s_env *next;
}               t_env;

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
    int aggr_index;
    char *data;
    t_type type;

    bool args_status; //
    char **args; //
    struct s_lexer *next;
}               t_lexer;

/*
** Abstract syntax tree (execution)
*/

typedef struct s_miniast
{
    t_lexer **tokenz;


}              t_miniast;


/*
** principale functions
*/
void    source_sh(t_env *);
void    append_list(t_lexer **root, char *data, t_type type);
void    append_list_redi(t_lexer **root, char *data, t_type type);
void    print_list(t_lexer *token_list);
t_lexer    *lexer(char *buf);
t_quote *quote_handling(char *s, char quote, int start);

/*
** utils
*/
bool    is_quote(int c);
size_t  ft_strlen_char(char *s, char c);
char	*ft_strchr_blank(const char *str, int c);
char    valid_string_quot(char *str);
size_t last_node_check(t_lexer **tokenz, t_type type, int node_index);

/*
** envirement
*/
t_env      *stock_env(char **env);


/*
** Redirections
*/

char **split_redir(char *str, int i);



/*
** built-ins
*/






/*
** expansions and Environment
*/





/*
** error handling
*/
char *error_ret(char *addr, char *err_message);

#endif