/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 17:41:00 by macos             #+#    #+#             */
/*   Updated: 2020/12/06 16:30:41 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static size_t countall_arr(char **agg, size_t delime_len) // calc all characters in **agg
{
    int len;
    int i;

    if (agg)
    {
        len = 0;
        i = 0;
        while (agg[i])
        {
            if (i > 0 && ft_strequ(agg[i - 1], "<<")) // agg[i] = text from here_doc
                len = (len - ft_strlen(agg[i])) + delime_len + 1;
            len = len + ft_strlen(agg[i]);
            i++;
        }
        return (len);
    }
    return (-1);
}

char    *redirection_varname(char ***arr, char *str, int *i)
{
    size_t c_len;
    char **agg;
    char *tmp = NULL;

    c_len = 0;
    agg = *arr;
    while(!is_blank(str[*i + c_len]) && str[*i + c_len] != ';' && str[*i + c_len] != '|' && str[*i + c_len] && !ft_is_there(AGG_REDI, str[*i + c_len])) // word expansions needed!!
        c_len++;
    if (str[*i + c_len] == '&')
        error_message("21sh: Error[1]: control jobs not handled yet...\n", 1);
    tmp = ft_strsub(str, *i, c_len);
    *i = *i + c_len;
    return (tmp);
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
    size_t delim_len = 0;

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
            {
                i++;
                g_agg_len++;
            }
            if (ft_is_there(";", str[i]))
                return (agg);
            if (!(agg[j] = ft_strnew(agg_len_str)) || i >= len)
                return (NULL);
            if ((str[i] == '>' || str[i] == '<') && str[i] == str[i + 1]) // for  >> or << right_fd
            {
                if (i + 2 < len && str[i + 2] == str[i] || str[i + 2] == ';' || str[i + 2] == '|')
                {
                    ft_free_arr(agg);
                    err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
                    return (NULL);
                }
                else
                {
                    agg[j][0] = str[i];
                    agg[j][1] = str[i];
                    // incr
                    active_word = 0;
                    j++;
                    i = i + 2;
                    continue ;
                }
                //g_agg_len += 2;
            }
            else if (str[i] == '&') // for &??? &> >&- <&- &>-
            {
                if (ft_isascii(str[i + 1]) && str[i + 1] != '<' && str[i +1] != '>' && str[i + 1] != '-')
                {
                    ft_free_arr(agg);
                    err_ret("21sh: syntax error near unexpected token `&'\n", NULL);
                    return (NULL);
                }
                agg[j][0] = str[i];
                if (i + 1 < len && (str[i + 1] == '>' || str[i + 1] == '<')) // for &
                {
                    agg[j][1] = str[i + 1];
                    active_word = 0;
                    if (i + 2 < len && (str[i + 2] == str[i + 1] || str[i + 2] == '-'))
                    {
                        agg[j][2] = str[i + 2];
                        j++;
                        break ;
                        active_word = 1;
                        i++;
                    }
                    i++;
                }
                j++;
            }
            else if ((str[i] == '>' || str[i] == '<') || str[i + 1] == '&')// for >&? or <&?
            {
                if ((str[i] == '>' && str[i + 1] == '<') || (str[i] == '<' && str[i + 1] == '>') || str[i + 1] == ';' || !str[i + 1])
                {
                    ft_free_arr(agg);
                    err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
                    return (NULL);
                }
                agg[j][0] = str[i]; // > or <
                active_word = 0;
                if (i + 1 < len && str[i + 1] == '&')
                {
                    agg[j][1] = '&'; // &
                    if (i + 2 < len && str[i + 2] && str[i + 2] == '-') // for >&-   >&1     >&out.txt
                    {
                        agg[j][2] = '-'; // -
                        j++;
                        break ;
                        active_word = 1;
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
            else if ((ft_isalnum(str[i])) && j > 0 && ft_strequ(agg[j - 1], "<<") && !is_quote(str[i])) // HERE_DOCUMENT
            {
                char *delim = ft_strndup(str + i, ft_strlen_char2(str + i, ' ', ';'));
                char *text = NULL;
                if (delim)
                    text = here_doc(delim);
                if (text == NULL)
                    return (NULL);
                if (text)
                {
                    free(agg[j]);
                    agg[j++] = text;
                    active_word = 1;
                }
                delim_len = ft_strlen(delim);
                if (str[i + delim_len - 1] == ';')
                    delim_len--;
                ft_strdel(&delim);
                break ;
            }
            else if (ft_isascii(str[i]) && (i < len) && !active_word &&(!ft_is_there(AGG_REDI, str[i + 1]) || !str[i + 1]))
            { // {varname}
                ft_strdel(&agg[j]);
                agg[j] = redirection_varname(&agg, str, &i);
                j++;
                active_word = 1;
                break ;
            }
            else if (ft_isascii(str[i]) && (i < len) && !active_word && ft_is_there(AGG_REDI, str[i + 1]))
            {// varneme
                free(agg[j]);
                agg[j] = redirection_varname(&agg, str, &i);
                j++;
                active_word = 1;
                break ;
            }
            else if (ft_isascii(str[i]) && active_word) // breaker
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
    g_agg_len += countall_arr(agg, delim_len);
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

// static int check_redirection_order(char *s1, char *s2)
// {
//     if (ft_isdigit(s1[0]) && !ft_is_there(AGG_REDI, s2[0]))
//         return (0);
//     else if (ft_is_there(AGG_REDI, s1[0]) && !ft_isascii(s2[0]))
//         return (0);
//     return (1);
// }

size_t     redirerction_parse(t_lexer **token_node, char **agg, t_pointt *cor, int *i_p) // ls >a>b>c
{
    int i;
    int j;
    size_t biglen;
    char *delim;

    j = 0;
    i = *i_p;
    while (agg[j] != NULL && agg[j][0] != '\0')
    {
        if (ft_isdigit(agg[j][0]) && j == 0 && i >= 1) // i >= 2
            append_list_redi(token_node, ft_strdup(agg[j]), L_REDIR, cor);
        else if (ft_is_there(AGG_REDI, agg[j][0]))
            append_list_redi(token_node, ft_strdup(agg[j]), AGGR_SYM, cor);
        else if (ft_isascii(agg[j][0])) // right_fd
            append_list_redi(token_node, ft_strdup(agg[j]), R_REDIR, cor);
        j++;
    }
    ft_free_arr(agg);
    return (g_agg_len);
}