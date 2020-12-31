/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_readline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 11:47:27 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	ft_ctl_l(t_line *line, char *str)
{
	tputs(tgoto(tgetstr("cm", 0), 0, 0), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
	ft_prompte();
	line->c_o.y = 0;
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	print_line(str);
	move_cursor_v(line);
	cur_goto(line, line->cursor);
}

void	cur_goto(t_line *line, int cursor)
{
	int x;
	int y;

	y = (line->c_v.y + (line->c_v.x + cursor) / line->col);
	x = ((line->c_v.x + cursor) % line->col);
	tputs(tgoto(tgetstr("cm", 0), x, y), 0, ft_output);
}

void	ft_put_multistring(char **strings)
{
	while (*strings)
		ft_putstr(*strings++);
}

void	print_line(char *str)
{
	while (str && *str)
	{
		if (*str == '\n')
			ft_putchar(' ');
		ft_putchar(*str);
		str++;
	}
	ft_putchar(' ');
}

void	ft_clear(t_line *line, char *str)
{
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
	print_line(str);
	ft_update_cursor_o(line);
	cur_goto(line, line->cursor);
}
