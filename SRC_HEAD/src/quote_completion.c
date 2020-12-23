/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_completion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 18:57:25 by macos             #+#    #+#             */
/*   Updated: 2020/12/23 05:53:38 by macos            ###   ########.fr       */
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

static char	*ft_strchr_quote(const char *s, int c)
{
    const char *addr;

    addr = s;
	while (*s)
	{
        if (*s == (char)c && *(s + 1) == '\0')
            return ((char *)s);
		if (*s == (char)c && (s > addr && *(s - 1) != '\\'))
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0' && *s == '\0')
		return ((char *)s);
	return (NULL);
}

t_quote    *quote_completion(t_quote **data, char quote)
{
    t_quote *my_quot;
    char *str;
    char *string;
    char *new_buff;
    char *tmp_string;

    new_buff = NULL;
    str = NULL;
    string = NULL;
    if (data) // quote_completion_check(t_quote ** data)
    {
        tmp_string = (*data)->string;
        if (tmp_string)
            string = ft_strjoin(tmp_string, "\n");
        if (tmp_string)
            ft_strdel(&tmp_string);
    }
    while (string)
    {
        if (g_clt_c || g_clt_D)
            return (NULL);
        prompt_completion(quote);
        if (!(new_buff = ft_readline(1)))
            return (NULL);
        add_to_his(new_buff, &g_his, 1);
        if (!ft_strchr(new_buff, quote) || (new_buff[0] == quote && !new_buff[1]))
            ft_str_append(&new_buff, "\n");
        tmp_string = string;
        if (string && new_buff)
            string = ft_strjoin(string, new_buff);
        if (tmp_string)
            ft_strdel(&tmp_string);
        if (ft_strchr(new_buff, quote) && string)
        {
            t_quote *tmp = *data;
            my_quot = quote_handling(string, quote, 1);
            if (my_quot)
            {
                ft_memdel((void**)&tmp);
                ft_strdel(&new_buff);
                ft_strdel(&string);
                return ((my_quot));
            }
        }
        ft_strdel(&new_buff);
    }
    return (*data);
}
