/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 03:00:47 by yabakhar          #+#    #+#             */
/*   Updated: 2020/11/21 19:43:36 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

void ft_copie(t_line *line, char *str)
{
    if (line->sltstr)
        ft_strdel(&line->sltstr);
    if (line->c_len > line->slctd)
    {
        line->slctf = (line->c_len - line->slctd);
        line->sltstr = ft_strsub(str, line->slctd, line->slctf + 1);
    }
    else
    {
        line->slctf = (line->c_len - line->slctd) * -1;
        line->sltstr = ft_strsub(str, line->c_len + 1, line->slctf);
    }
    line->slct = 0;
    line->slctd = 0;
    ft_clear(line, str);
}

void ft_chack_selction(t_line *line, char *str)
{
    if (line->slct)
    {
        line->slct = 0;
        line->slctd = 0;
        ft_clear(line, str);
    }
    else if (!line->slct)
    {
        line->slctf = 0;
        line->slctd = line->c_len;
        line->slct = 1;
    }
}

void ft_select(t_line *line, char *str)
{
    if (str)
    {
        tputs(tgetstr("mr", 0), 0, ft_output);
        ft_putchar(str[line->c_len]);
        tputs(tgetstr("me", 0), 0, ft_output);
    }
}

void ft_unselect(t_line *line, char *str)
{
    if (str)
        ft_putchar(str[line->c_len]);
}