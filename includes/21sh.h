
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
# define METACHARACTER "|&;()<> "

typedef enum e_type
{
    WORD=1,
    METACHAR,
    PATH,
    AGGR,
    DQUOT,
    SQUOT,
}           t_type;

typedef struct s_env
{
    char *env_var_name;
    char *env_var_value;
    struct s_env *next;
}               t_env;

typedef struct s_quote
{
    char *string;
    size_t size;
}               t_quote;

typedef struct s_lexer
{
    char *data;
    t_type type;
    struct s_lexer *next;
}               t_lexer;



void    source_sh(t_env *);
void    append_list(t_lexer **root, char *data, t_type type);
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

/*
** envirement
*/
t_env      *stock_env(char **env);
#endif