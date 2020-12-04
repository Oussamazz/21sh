/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 17:41:00 by macos             #+#    #+#             */
/*   Updated: 2020/12/04 03:02:59 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"


char    *redirection_varname(char ***arr, char *str, int *i)
{
    size_t c_len;
    char **agg;
    char *tmp = NULL;

    c_len = 0;
    agg = *arr;
    while(!is_blank(str[*i + c_len]) && str[*i + c_len]) // word expansions needed!!
        c_len++;
    tmp = ft_strsub(str, *i, c_len);
    *i = *i + c_len;
    return (tmp);
}

static void redirection_varname_2(char ***arr, char *str, int *j, int *i, size_t len)
{
    char **agg;
    char *tmp = NULL;

    *j = *j + 1;
    *i = *i + 1;
    while(is_blank(str[*i]) && str[*i])
        *i = *i + 1;
    agg = *arr;
    tmp = ft_strsub(str, *i, len - *i + 1);
    agg[*j] = ft_strdup(tmp);
    ft_strdel(&tmp);
    *i = *i + len - (*i) - 1;
}

static char *get_digits(char *str, int i)
{
    int j;
    int k;
    char *s;

    s = ft_strnew(5);
    if (!s)
    {
        j = i;
        k = 0;
        while (str[j] && j >= 0 && k <= 4)
        {
            if (ft_isdigit(str[j]))
            {
                s[k] = str[j];
                k++;
            }
            else if (j >= 0 && k <= 0 && is_blank(str[j]))
            {
                ft_strdel(&s);
                return (err_ret(ERR_BAD_FD, NULL));
            }
            j--;
        }
        return (s);
    }
    return (NULL);
}

//  *str must be  at aggr_symbol {><&}
static char *get_left_fd(char *str, int pos)
{
    int i;
    int j;
    char *tmp;

    tmp = ft_strnew(ft_strlen(str));
    if (!tmp)
    {
        i = pos;
        j = 0;
        while (i >= 0 && str[i] && !is_blank(str[i]))
        {
            tmp[j] = str[i];
            i--;
            j++;
        }
        return (tmp);
    }
    return (NULL);
}

static int  calc_symbol(char *str)
{
    int count;

    count = 0;
    while (str[count] && ft_is_there(AGG_REDI, str[count]))
        count++;
    return (count);
}

char **split_redir(char *str, int pos)
{
    int symbol;
    int i;
    int j;
    char **agg;
    char *tmp;
    int active_word = 0;

    agg = NULL;
    size_t agg_len = wordinbuff_size(str) + 1; // 4
    size_t agg_len_str = 0;
    
    if (str && (agg = (char**)ft_memalloc(sizeof(char*) * agg_len)) != NULL) // word size must be calculated!! (agg_len?)
    {
        size_t len = ft_strlen(str);
        i = 0; // pos
        j = 0;
        //agg_len_str = wordinstr_size(str + i, agg_len - 1);
        agg_len_str = len;
        while (i < len && str[i] != '\0' && j < agg_len)
        {
            while (str[i] && is_blank(str[i]) && i < len)
                    i++;
            if (ft_is_there(";", str[i]))
                return (agg);
            if (!(agg[j] = ft_strnew(agg_len_str)) || i >= len)
                return (NULL);
            if ((str[i] == '>' || str[i] == '<') && str[i] == str[i + 1] && !ft_is_there(AGG_REDI, str[i + 2])) // for  >> or <<
            {
                agg[j][0] = str[i];
                agg[j][1] = str[i];
                j++;
                i = i + 2;
            }
            else if (str[i] == '&') // for &???
            {
                agg[j][0] = str[i];
                if (i + 1 < len && (str[i + 1] == '>' || str[i + 1] == '<')) // for &
                {
                    agg[j][1] = str[i + 1];
                    if (i + 2 < len && (str[i + 2] == str[i + 1] || str[i + 2] == '-'))
                    {
                        agg[j][2] = str[i + 2];
                        i++;
                    }
                    i++;
                }
                j++;
            }
            else if ((str[i] == '>' || str[i] == '<') && (!ft_is_there(AGG_REDI, str[i + 1]) || str[i + 1] == '&')) // for >&? or <&?
            {
                agg[j][0] = str[i]; // > or <
                if (i + 1 < len && str[i + 1] == '&')
                {
                    agg[j][1] = '&'; // &
                    if (i + 2 < len && str[i + 2] && str[i + 2] == '-') // for >&-   >&1     >&out.txt
                    {
                        agg[j][2] = '-'; // -
                        active_word = !active_word;
                        i++;
                    }
                    i++;
                }
                j++;
            }
            else if (is_quote(str[i]) && str[i - 1] != '\\')
            {
                ft_strdel(&agg[j]);
                return (agg);
            }
            else if ((ft_isalnum(str[i])) && ft_strequ(agg[j - 1], "<<") && !is_quote(str[i])) // HERE_DOCUMENT
            {
                char *delim = ft_strndup(str + i, ft_strlen_char(str + i, ' '));
                char *text = NULL;
                if (delim)
                    text = here_doc(delim);
                if (text == NULL)
                    return (NULL);
                if (text)
                    agg[j++] = text;
                ft_strdel(&delim);
                break ;
            }
            else if ((str[i] == '/' || ft_isalnum(str[i]) || str[i] == '$') && (i < len) && !active_word && !ft_is_there(AGG_REDI, str[i + 1]))
            { // {varname}
                ft_strdel(&agg[j]);
                agg[j++] = redirection_varname(&agg, str, &i);
                active_word = !active_word;
                break ;
            }
            else if ((ft_isalnum(str[i]) || str[i] == '$') && i < len && active_word)
                break ;
            else // error handling!
            {
                ft_free_arr(agg);
                err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
                return (NULL);
            }
            i++;
        }
    }
    agg[j] = NULL;
    return (agg);
}

static char *modify_right_redir(char **agg)
{
    size_t len;
    int i;
    char *str;
    char *tmp;

    str = NULL;
    if (agg)
    {
        i = 0;
        str = *agg;
        tmp = str;
        len = ft_strchr(str, ';') - str;
        str = ft_strsub(str, 0, len);
        ft_strdel(&tmp);       
    }
    return (str);
}

size_t     redirerction_parse(t_lexer **token_node, char **agg, t_pointt *cor, int *i_p)
{
    int i;
    int j;
    size_t biglen;

    j = 0;
    i = *i_p;
    while (agg[j] != NULL && agg[j][0] != '\0')
    {
        if (ft_isdigit(agg[j][0]) && j == 0 && i >= 2)
            append_list_redi(token_node, agg[j], L_REDIR, cor);
        else if (ft_is_there(AGG_REDI, agg[j][0]))
        {
            if (!j && agg[j][0] == '>')
                append_list_redi(token_node, agg[j], AGGR_SYM, cor);
            else if (!j && agg[j][0] == '<')
                append_list_redi(token_node, agg[j], AGGR_SYM, cor);
            else
                append_list_redi(token_node, agg[j], AGGR_SYM, cor);
        }
        else if (ft_isascii(agg[j][0]))
        {
            if (ft_is_there(agg[j], ';'))
                agg[j] = modify_right_redir(&agg[j]);
            append_list_redi(token_node, agg[j], R_REDIR, cor);
        }
        j++;
    } // 
    j = 0;
    biglen = 0; //  get biglen funct
    while (agg[j] != NULL)
    {
        biglen += ft_strlen(agg[j]);
        j++;
    }
    return (biglen);
}