/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 01:50:54 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/26 16:13:59 by oelazzou            ###   ########.fr       */
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


int  	aggr_function(char *buf, t_pointt *coord, t_lexer **token_node)
{
    char **agg;
    char *buf_dup;
    int i;

    buf_dup = NULL;
    agg = NULL;
    i = 0;
    if (!*(buf + 1))
    {
        ft_putendl_fd("21sh: syntax error near unexpected token `newline'", 2);
        return (-1);
    }
    if (!(agg = split_redir(buf)))
        return (-1);
    i = i + redirerction_parse(token_node, agg, coord) - 1;
    g_agg_len = 0;
    return (i);
}

int     quote_function(char *buf, t_lexer **token_node, t_pointt *coord, char *quote)
{
    int i;
    int flag;
    char *quote_cnt;
    char c;

    i = 0;
    flag = quote != NULL;
    c = quote ?  *quote : *buf;
    if(!quote)
    {
        buf++;
        i += 2;
    }
    if (!(quote_cnt = get_content_quote(buf, c, coord, flag)))
        return (-1);
    if (c == '\'')
        append_list(token_node, quote_cnt, SQUOT, coord);
    else if (c == '\"')
        append_list(token_node, quote_cnt, DQUOT, coord);
    i += (int)ft_strlen(quote_cnt);
    ft_strdel(&quote_cnt);
    coord->no_space = 0;
    return (i);
}

int     get_the_word(char *buf, t_lexer **token_node, t_pointt *coord)
{
    char tmp[MIN_INDEX];
    int j;

    j = 0;
    while (buf[j] && !ft_is_there(METACHARACTER, buf[j]) && !ft_is_aggr(buf[j]) && buf[j] != '|' && buf[j] != '$')
    {
        tmp[j] = buf[j];
        j++;
    }
    if (buf[j] == '$')
        coord->no_space = 1;
    tmp[j] = '\0';
    append_list(token_node, tmp, WORD, coord);
    ft_strclr(tmp);
    return (j);
}

int		simple_word_function(char *buf, t_lexer **token_node, t_pointt *coord, size_t buf_len)
{
    int i;
    char *temp;

    temp = NULL;
    i = 0;
    if (*buf && ft_isdigit(*buf) && ft_is_there(AGG_REDI, *(buf + 1)) && *(buf + 1) != '\0')
    {
        temp = get_left_fd_(buf);
        append_list_redi(token_node, ft_strdup(temp), L_REDIR, coord);
        i = ft_strlen(temp);
        ft_strdel(&temp);
    }
    else if (*buf)
        i = get_the_word(buf, token_node, coord);
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