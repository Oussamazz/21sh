/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 11:03:56 by macos             #+#    #+#             */
/*   Updated: 2020/11/30 11:24:25 by macos            ###   ########.fr       */
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
    ft_put_multistring2((char *[]){"\033[1;31m >", "\033[0m", 0, 0, 0, 0, 0});
}

char    *here_doc(char *delim)
{
    char *text;
    char *tmp;
    char *buffer;

    text = ft_strdup("");
    buffer = ft_strnew(MAX_INDEX);
    while (delim && buffer)
    {
        prompt_here_doc();
        if(!(buffer = ft_readline()))
            return ;
        if (ft_strequ(buffer, delim))
            break ;
        buffer = ft_strcat(buffer, "\n");
        tmp = text;
        if(!(text = ft_strjoin(text, buffer)))
            return (NULL);
        if (tmp)
            ft_strdel(&tmp);
        ft_strclr(buffer);
    }
    return (text);
}