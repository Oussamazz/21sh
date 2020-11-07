/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 17:41:00 by macos             #+#    #+#             */
/*   Updated: 2020/11/05 11:41:28 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"


static void redirection_varname(char ***arr, char *str, int *j, int *i)
{
    size_t c_len;
    char **agg;
    char *tmp = NULL;

    c_len = 0;
    agg = *arr;
    ft_strdel(&(agg[*j]));
    while(!is_blank(str[*i + c_len]) && str[*i + c_len]) // word expansions needed!!
        c_len = c_len + 1;
    tmp = ft_strsub(str, *i, c_len);
    agg[*j] = tmp;
    *j = *j + 1;
    *i = *i + c_len;
    return ;
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
                return (error_ret(NULL, ERR_BAD_FD));
            }
            j--;
        }
        return (s);
    }
    return (NULL);
}

// we must be  (*str) at aggr_symbol {><&}
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
    int agg_len = 10;
    if (str && (agg = (char**)ft_memalloc(sizeof(char*) * agg_len)) != NULL) // word size must be calculated!! (agg_len?)
    {
        size_t len = ft_strlen(str + pos);
        i = 0;
        j = 0;
        if (pos >= 1 && str[pos - 1])// pos - 1 
        {
            if (str[pos - 1] && is_blank(str[pos - 1]))
            {
                symbol = calc_symbol(str + pos);
                agg[0] = ft_strsub(str, pos, symbol);
                agg[1] = NULL;
                return (agg);
            }
            else if (ft_isdigit(str[pos - 1]) && (agg[0] = get_digits(str, pos - 1)) != NULL)
                return (agg);
            else
            {
                agg[0] = get_left_fd(str, pos);
                agg[1] = NULL;
                return (agg);
            }
        } 
        while (str[i] != '\0' && j < agg_len && i < len)
        {
            if (!(agg[j] = ft_strnew(agg_len)))
                return NULL;
            while (is_blank(str[i]) && i < len)
                i++;
            if ((str[i] == '>' || str[i] == '<') && (str[i + 1] == '>' || str[i + 1] == '<') && str[i] == str[i + 1])
            {
                agg[j][0] = str[i];
                agg[j][1] = str[i];
                j++;
                i++;
            }
            else if (ft_isdigit(str[i]) || str[i] == '>' || str[i] == '<')
            {
                agg[j][0] = str[i];
                if (i + 1 < len && str[i + 1] == '&' && (str[i] == '>' || str[i] == '<')) // for &
                {
                    agg[j][1] = '&';
                    if (i + 2 < len && str[i + 2] && str[i + 2] == '-')
                    {
                        agg[j][2] = '-';
                        i++;
                    }
                    i++;
                }
                else if (str[i] == '>' || str[i] == '<' && str[i + 1])
                    //printf("i = %d || str[%d] == %c\n ", i, i, str[i]);
                    redirection_varname_2(&agg, str, &j, &i, len);
                j++;
            }
            else if (ft_isalpha(str[i]) && i < len && str[i - 1] != '&' && !ft_isdigit(str[i - 1]) && !active_word)
            { // {varname}
                redirection_varname(&agg, str, &j, &i);
                active_word = !active_word;
            }
            else // error handling!
            {
                ft_putstr_fd("Error bad file descriptor\n", 2);
                exit(1);
            }
            i++;
        }
    }
    return (agg);
}