/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 21:12:01 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/24 18:44:32 by oelazzou         ###   ########.fr       */
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
        if (!ft_strcmp(cmd[i], "$$"))
            ft_putnbr_fd((int)getppid(), 1);
        else
            ft_putstr_fd(cmd[i], 1);
        if (cmd[i + 1])
            ft_putchar_fd(' ', 1);
    }
    if (!flag)
        ft_putchar_fd('\n', 1);
}
