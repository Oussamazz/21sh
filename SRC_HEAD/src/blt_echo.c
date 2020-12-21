/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 21:12:01 by macos             #+#    #+#             */
/*   Updated: 2020/12/21 04:19:21 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    blt_echo(char **cmd)
{
    int i;
    int flag;

    i = 0;
    flag = 0;
    if (cmd[1] && ft_strequ(cmd[1], "-n"))
    {
        i++;
        flag = 1;
    }
    while (cmd[++i] != NULL)
    {
        ft_putstr_fd(cmd[i], 1);
        if (cmd[i + 1])
            ft_putchar_fd(' ', 1);
    }
    if (!flag)
        ft_putchar_fd('\n', 1);
}

// echo $HOME$PWD


//  echo = WORD
//  $HOME = /Users/oelazzou
//  $PWD = xterm-256
