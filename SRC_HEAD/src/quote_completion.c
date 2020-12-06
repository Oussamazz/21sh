/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_completion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 18:57:25 by macos             #+#    #+#             */
/*   Updated: 2020/12/06 16:59:52 by macos            ###   ########.fr       */
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
    string = NULL;
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
            return (NULL);
        if (!ft_strchr(new_buff, quote) || (new_buff[0] == quote && !new_buff[1]))
            ft_str_append(&new_buff, "\n");
        tmp_string = string;
        if (string && new_buff)
            string = ft_strjoin(string, new_buff);
        if (tmp_string)
            ft_strdel(&tmp_string);
        if (ft_strchr(new_buff, quote) && string)
        {
            my_quot = quote_handling(string, quote, 1, env_list);
            if (my_quot)
                return ((my_quot));
        }
        ft_strdel(&new_buff);
    }
    return (*data);
}
