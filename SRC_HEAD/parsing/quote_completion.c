/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_completion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 18:57:25 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/26 15:26:16 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"


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
