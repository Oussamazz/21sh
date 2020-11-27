/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_completion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 18:57:25 by macos             #+#    #+#             */
/*   Updated: 2020/11/27 03:22:20 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void ft_put_multistring2(char **strings)
{
	while (*strings)
		ft_putstr(*strings++);
}

static void     prompt_completion(char quot)
{
    if (quot == '\"')
        ft_put_multistring2((char *[]){"\033[1;31m\"", "\033[0m", "\033[1;32m>", "\033[0m", 0, 0, 0});
    else
        ft_put_multistring2((char *[]){"\033[1;31m\'","\033[0m", "\033[1;32m>", "\033[0m", 0, 0, 0});
    return ;
}

static void ft_str_append(char **buf, char *append)
{
    char *new_buf;

    new_buf = NULL;
    if (buf && *buf)
    {
        new_buf = ft_strjoin(*buf, append);
        ft_strdel(&(*buf));
        *buf = new_buf;
    }
    return ;
}

t_quote    *quote_completion(t_quote **data, char quote, t_env **env_list)
{
    t_quote *my_quot;
    char *str;
    char *string;
    int flag;
    char *new_buff;
    char *tmp_string;
    char *pointer;

    new_buff = NULL;
    str = NULL;
    if (data)
    {
        tmp_string = (*data)->string;
        string = ft_strjoin(tmp_string, "\n");
        if (tmp_string)
            ft_strdel(&tmp_string);
    }
    flag = 1;
    while (flag && string)
    {
        prompt_completion(quote);
        if (!(new_buff = ft_readline()))
            return (*data);
        ft_str_append(&new_buff, "\n");
        tmp_string = str;
        if (string && new_buff)
            str = ft_strjoin(string, new_buff);
        ft_strdel(&tmp_string);
        if ((pointer =ft_strchr(str, quote)))
        {
            //my_quot = quote_handling(str, quote, 1, env_list);
            return ((*data = quote_handling(str, quote, 1, env_list)));
        }
        ft_strdel(&new_buff);
        string = str;
    }
    return (*data);
}
