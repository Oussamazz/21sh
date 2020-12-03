/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 11:03:56 by macos             #+#    #+#             */
/*   Updated: 2020/12/01 19:06:50 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void ft_put_multistring2(char **strings)
{
	while (*strings)
		ft_putstr(*strings++);
}

void     prompt_here_doc()
{
    ft_put_multistring2((char *[]){"\033[1;31m>", "\033[0m", 0, 0, 0, 0, 0});
}

char    *here_doc(char *delim)
{
    int flag;
    char *text;
    char *tmp;
    char *buffer;

    text = NULL;
    buffer = NULL;
    flag = 0;
    while (delim)
    {
        prompt_here_doc();
        if(!(buffer = ft_readline()))
            return (NULL);
        if (ft_strequ(buffer, delim))
        {
            ft_strdel(&buffer);
            return (text);
        }
        buffer = ft_strcat(buffer, "\n");
        tmp = text;
        if (!flag && buffer)
        {
            if(!(text = ft_strdup(buffer)))
                return (NULL);
            flag = 1;
            ft_strdel(&buffer);
            continue ;
        }
        if (!(text = ft_strjoin(text, buffer)))
            return (NULL);
        if (flag && tmp)
            ft_strdel(&tmp);
        ft_strdel(&buffer);
    }
    return (text);
}