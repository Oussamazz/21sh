/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 01:33:51 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/30 18:23:36 by oelazzou         ###   ########.fr       */
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

char *redirection_varname(char ***arr, char *str, size_t *i)
{
    size_t c_len;
    char **agg;
    char *tmp = NULL;

    c_len = 0;
    agg = *arr;
    while (!is_blank(str[*i + c_len]) && str[*i + c_len] != ';' && str[*i + c_len] != '|' && str[*i + c_len] && !ft_is_there(AGG_REDI, str[*i + c_len])) // word expansions needed!!
        c_len++;
    if (str[*i + c_len] == '&')
        error_message("21sh: Error[1]: control jobs not handled yet...\n", 1);
    tmp = ft_strsub(str, *i, c_len);
    *i = *i + c_len;
    return (tmp);
}

static int split_agg(t_split_redir *v, char *str)
{
    if ((str[v->i] == '>' && str[v->i + 1] == '<') || (str[v->i] == '<' && str[v->i + 1] == '>') || str[v->i + 1] == ';' || !str[v->i + 1])
    {
        ft_free_arr(v->agg);
        err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
        return (ReturnNull);
    }
    v->agg[v->j][0] = str[v->i]; // > or <
    v->active_word = 0;
    if (v->i + 1 < v->len && str[v->i + 1] == '&')
    {
        v->agg[v->j][1] = '&';                                          // &
        if (v->i + 2 < v->len && str[v->i + 2] == '-') // for >&-   >&1     >&out.txt
        {
            v->agg[v->j][2] = '-'; // -
            v->j++;
            return (Break);
        }
        v->i++;
    }
    v->j++;
    return (Normal);
}

static int split_herdoc2(t_split_redir *v, char *str)
{
    char *delim = ft_strndup(str + v->i, ft_strlen_char2(str + v->i, ' ', ';'));
    char *text = NULL;
    if (delim)
        text = here_doc(delim);
    if (text == NULL)
        return (ReturnNull);
    if (text)
    {
        free(v->agg[v->j]);
        v->agg[v->j++] = text;
        v->active_word = 1;
    }
    v->delim_len = ft_strlen(delim);
    if (str[v->i + v->delim_len - 1] == ';')
        v->delim_len--;
    ft_strdel(&delim);
    return (Break);
}

static int split_herdoc(t_split_redir *v, char *str)
{
    if ((v->i + 2 < v->len) && ((str[v->i + 2] == str[v->i]) || (str[v->i + 2] == ';' || str[v->i + 2] == '|')))
    {
        ft_free_arr(v->agg);
        err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
        return (ReturnNull); //return NULL
    }
    v->agg[v->j][0] = str[v->i];
    v->agg[v->j][1] = str[v->i];
    v->active_word = 0;
    v->j++;
    v->i = v->i + 2;
    return (Continue); //continue
}

static int split_redir_fd(t_split_redir *v, char *str)
{
    if (ft_isascii(str[v->i + 1]) && str[v->i + 1] != '<' && str[v->i + 1] != '>' && str[v->i + 1] != '-')
    {
        ft_free_arr(v->agg);
        err_ret("21sh: syntax error near unexpected token `&'\n", NULL);
        return (ReturnNull);
    }
    v->agg[v->j][0] = str[v->i];
    if (v->i + 1 < v->len && (str[v->i + 1] == '>' || str[v->i + 1] == '<')) // for &
    {
        v->agg[v->j][1] = str[v->i + 1];
        v->active_word = 0;
        if (v->i + 2 < v->len && (str[v->i + 2] == str[v->i + 1] || str[v->i + 2] == '-'))
        {
            v->agg[v->j][2] = str[v->i + 2];
            v->j++;
            return (Break); //break
        }
        v->i++;
    }
    v->j++;
    return (Normal); //do nothing
}

static int split_varname(t_split_redir *v, char *str)
{
    ft_strdel(&v->agg[v->j]);
    v->agg[v->j] = redirection_varname(&v->agg, str, &v->i);
    v->j++;
    v->active_word = 1;
    return (Break);
}

static int do_spliting(t_split_redir *v, char *str)
{
    while (str[v->i] && is_blank(str[v->i]) && ++v->i)
        g_agg_len++;
    if (str[v->i] == ';')
        return (Returnagg);
    if (!(v->agg[v->j] = ft_strnew(v->len)) || v->i >= v->len)
        return (ReturnNull);
    if ((str[v->i] == '>' || str[v->i] == '<') && str[v->i] == str[v->i + 1]) // for  >> or << right_fd 
        return (split_herdoc(v, str));
    if (str[v->i] == '&')
        return (split_redir_fd(v, str));                                // ya ima retunr ola break ola kamal 3adi
    if ((str[v->i] == '>' || str[v->i] == '<') || str[v->i + 1] == '&') // for >&? or <&?
        return (split_agg(v, str));
    if (is_quote(str[v->i]))
    {
        ft_strdel(&v->agg[v->j]);
        return (Returnagg);
    }
    if ((ft_isalnum(str[v->i])) && v->j > 0 && ft_strequ(v->agg[v->j - 1], "<<") && !is_quote(str[v->i])) // HERE_DOCUMENT
        return (split_herdoc2(v, str));
    if (ft_isascii(str[v->i]) && (v->i < v->len) && !v->active_word) //&& (!ft_is_there(AGG_REDI, str[v->i + 1]) || !str[v->i + 1])
        return (split_varname(v, str));
    if (ft_isascii(str[v->i]) && v->active_word) // breaker
        return (Break);
    return (Normal);
}

// >>         test | ls . sdfsa 2>&- > dirlist

int     len_of_redir(char *str)
{
    int count;
    int active_word;

    count = 0;
    active_word = 0;
    while (*str)
    {
        while (*str && is_blank(*str))
            str++;
        if (ft_is_there(AGG_REDI, *str) && *str)
            count++;
        else if (ft_isalnum(*str) && !active_word)
	    {
		    active_word = 1;
            while (ft_isalnum(*str))
            {
                count++;
                str++;
            }
	    }
	    if (!*str || (ft_is_there(AGG_REDI, *str) && active_word))
		    break ;
        str++;
    }
    return (count);
}

char **split_redir(char *str)
{
    t_split_redir v;

    ft_bzero(&v, sizeof(t_split_redir));
    v.agg_len = 3; // word_count
    if (str && (v.agg = (char **)ft_memalloc(sizeof(char *) * v.agg_len)))
    {
        v.len = ft_strlen(str);
        while (v.i < v.len  && str[v.i] != '\0' && v.j < v.agg_len)
        {
            v.status = do_spliting(&v, str);
            if (v.status == Returnagg)
                return (v.agg);
            if (v.status == ReturnNull)
                return (NULL);
            if (v.status == Break)
                break;
            if (v.status == Continue)
                continue;
            v.i++;
        }
    }
    v.agg[v.j] = NULL;
    g_agg_len += countall_arr(v.agg, v.delim_len);
    return (v.agg);
}