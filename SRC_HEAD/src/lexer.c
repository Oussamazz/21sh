/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 01:50:54 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/22 05:02:00 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int     expansion_function(char *buf, t_lexer **token_node, t_pointt *coord, t_env **env_list)
{
    int i;

    i = 0;
    if (buf[i] == '$' && (buf[i + 1] == '(' || buf[i + 1] == ')'))
    {
        ft_putendl_fd("21sh: Unexpected token `( or )'", 2);
        return (-1);
    }
    return (expansion_parse(token_node, buf + i, env_list, coord));
}

int   sep_function(char *buf, t_lexer **token_node, t_pointt *coord)
{
    int i;

    i = 0;
    coord->pipe_index = 1;
    coord->aggr_index= 1;
    append_list(token_node, ";", SEP, coord);
    return (1);
}

int  	aggr_function(char *buf, t_pointt *coord, t_lexer **token_node, int *i)
{
    char **agg;
    char *buf_dup;

    buf_dup = NULL;
    agg = NULL;
    if (!buf[*i + 1])
    {
        ft_putendl_fd("21sh: Unexpected token.", 2);
        return (-1);
    }
    buf_dup = ft_strdup(buf + *i);
    if (!(agg = split_redir(buf_dup, *i)))
        return (-1);
    *i = *i + redirerction_parse(token_node, agg, coord, i) - 1;
    g_agg_len = 0;
    ft_strdel(&buf_dup);
    return (0);
}

int     quote_function(char *buf, t_lexer **token_node, t_pointt *coord, t_env **env_list)
{
    int i;
    t_quote *quot;

    quot = NULL;
    i = 0;
    if (buf[i] == '$')
        i++;
    if (!(quot = quote_handling(buf + i + 1, buf[i], 1, env_list)))
        return (i += 1);
    if (quot->string && (buf[i] == '\'' || buf[i] == '\"') && ret_last_node_type(token_node) == AGGR_SYM)
        append_list_redi(token_node, ft_strdup(quot->string), R_REDIR, coord);
    else if (buf[i] == '\'')
        append_list(token_node, quot->string, SQUOT, coord);
    else
        append_list(token_node, quot->string, DQUOT, coord);
    i += quot->size; // -1
    ft_strdel(&(quot->string));
    ft_memdel((void**)&quot);
    return (i);
}

int     quote_handling_function(t_lexer **token_node, t_quote *quot, char q, t_pointt *coord)
{
    int i;

    i = 0;
    if (!quot)
        return (1);
    if (q == '\'')
        append_list(token_node, quot->string, SQUOT, coord);
    else
        append_list(token_node, quot->string, DQUOT, coord);
    i += quot->size;
    ft_strdel(&quot->string);
    ft_memdel((void**)&quot);
    return (i);
}

int		simple_word_function(char *buf, t_lexer **token_node, t_pointt *coord, size_t buf_len)
{
    int i;
    int j;
    char tmp[1024];
    char *temp;

    i = 0;
    temp = NULL;
    if (buf[i] && ft_isdigit(buf[i]) && ft_is_there(AGG_REDI, buf[i + 1]) && buf[i + 1] != '\0')
    {
        temp = get_left_fd_(buf + i);
        append_list_redi(token_node, ft_strdup(temp), L_REDIR, coord);
        i = i + ft_strlen(temp) - 1;
        ft_strdel(&temp);
    }
    else if (buf + i)
    {
        j = 0;
        while (buf[i + j] && !ft_is_there(METACHARACTER, buf[i + j]) && !ft_is_aggr(buf[i + j]) && buf[i + j] != '|')
        {
            if (buf[i + j] == '\\')
            {
                if (buf[i + j + 1] == '\\')
                {
                    tmp[j] = buf[i + j];
                    j++;
                }
                i++;
                continue ;
            }
            tmp[j] = buf[i + j];
            j++;
        }
        tmp[j] = '\0';
        append_list(token_node, tmp, WORD, coord);
        if (buf[i + j] == ';' || buf[i + j] == '<' || buf[i + j] == '>' || buf[i + j] == '|')
            i--;
        i = i + ft_strlen(tmp);
        ft_strclr(tmp);
    }
    return (i);
}

int     meta_function(char *buf, t_lexer **token_node, t_pointt *coord)
{
    int i;
    int j;
    char tmp[1024];

    j = 0;
    i = 0;
    while (ft_is_there(METACHARACTER, buf[i + j]) && !is_blank(buf[i + j]) && buf[i + j])
    {
        tmp[j] = buf[i + j];
        j++;
    }
    append_list(token_node, tmp, METACHAR, coord);
    i = i + ft_strlen(tmp) - 1;
    ft_strclr(tmp);
    return (i);
}